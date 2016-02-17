#include "AuditannotateCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace clang::ast_matchers;

namespace clang {
	namespace tidy {
		namespace readability {

			const char VarDeclWithAutoId[] = "decl_auto";
			const char AllImplicitCastExprsId[] = "implicit_casts";
			const char LambdaExprCapturesId[] = "lambda_captures";

			DeclarationMatcher makeAutoDeclMatcher()
			{
				return varDecl(
						anyOf(
							hasType(autoType()),
							hasType(pointerType(pointee(autoType())))
							)
						).bind(VarDeclWithAutoId);
			}

			void AuditannotateCheck::replaceUsesOfAuto(const VarDecl *V)
			{
				// Implicit declarations are generated implicitly by the implementation and should be ignored.
				if (V->isImplicit())
					return;
				

				auto VT = V->getType().getCanonicalType();

				// Exclude lambdas because clang doesn't give good types for them
				//    The output looks something like this: class (lambda at /Users/ryan/svn/llvm/test/test2.cpp:44:15)
				const Type *VTT = VT.getTypePtr();
				if (const CXXRecordDecl *CXXRD = VTT->getAsCXXRecordDecl())
					if (CXXRD->isLambda())
						return;

				SourceRange Range(V->getTypeSourceInfo()->getTypeLoc().getSourceRange());
				auto Diag = diag(Range.getBegin(), "auto makes it difficult to do security audits");

				Diag << FixItHint::CreateReplacement(Range, VT.getAsString());
			}

			void AuditannotateCheck::elaborateOnImplicitCasts(const ImplicitCastExpr *I)
			{
				switch (I->getCastKind())
				{
					case CastKind::CK_Dependent:
					case CastKind::CK_IntegralCast:
					case CastKind::CK_IntegralToPointer:
					case CastKind::CK_PointerToIntegral:
					case CastKind::CK_BitCast:
					case CastKind::CK_UncheckedDerivedToBase:
					case CastKind::CK_ToUnion:
					case CastKind::CK_UserDefinedConversion:
					case CastKind::CK_AtomicToNonAtomic:
					case CastKind::CK_NonAtomicToAtomic:
						break;
					default:
						return;
				}


				const QualType dest_type = I->getType().getCanonicalType();
				const Expr *E = I->getSubExpr();

				if (isa<IntegerLiteral>(E))
					return;

				const QualType source_type = E->getType().getCanonicalType();

				SourceRange Range(I->getSourceRange());
				auto Diag = diag(Range.getBegin(), "implicit cast");

				std::string comment_string = "/* CAST:" + source_type.getAsString() + "->" + dest_type.getAsString() + "*/";
				Diag << FixItHint::CreateInsertion(Range.getBegin(), comment_string);

			}

			bool AuditannotateCheck::typeHasPointerMembers(const Type *V)
			{
				if (V->isAnyPointerType())
					return true;

				// Recursively unpack structures looking for any pointer members
				// XXX: Need to do classes and unions also
				if (V->isStructureType())
				{
					const RecordType *ST = V->getAsStructureType();
					const RecordDecl *STD = ST->getDecl()->getDefinition();

					for (auto F : STD->fields())
					{
						const QualType FT = F->getType().getCanonicalType();
						const Type *FTT = FT.getTypePtr();
						if (typeHasPointerMembers(FTT))
							return true;
					}

				}


				return false;
			}

			void AuditannotateCheck::annotateLambaPointerCaptures(const LambdaExpr *L)
			{
					for (const auto C : L->captures())
					{
						// XXX: check for VLA captures
						//
						// XXX: could probably add `|| C.capturesThis()` to this statement but I don't have a test
						if (C.capturesVariable())
						{
							const VarDecl *CV = C.getCapturedVar();
							const QualType VT = CV->getType().getCanonicalType();

							const Type *VTT = VT.getTypePtr();
							if (typeHasPointerMembers(VTT))
							{
								auto Diag = diag(C.getLocation(), "captured pointer");

								std::string comment_string = "/* captured pointer */";
								Diag << FixItHint::CreateInsertion(C.getLocation(), comment_string);
							}
						}
					}

			}


			void AuditannotateCheck::registerMatchers(ast_matchers::MatchFinder *Finder)
			{
				Finder->addMatcher(makeAutoDeclMatcher(), this);
				Finder->addMatcher(lambdaExpr().bind(LambdaExprCapturesId), this);
				Finder->addMatcher(implicitCastExpr().bind(AllImplicitCastExprsId), this);
			}

			void AuditannotateCheck::check(const MatchFinder::MatchResult &Result) 
			{
				if (const auto *V = Result.Nodes.getNodeAs<VarDecl>(VarDeclWithAutoId))
				{
					replaceUsesOfAuto(V);
				}
				else if (const auto *I = Result.Nodes.getNodeAs<ImplicitCastExpr>(AllImplicitCastExprsId))
				{
					elaborateOnImplicitCasts(I);
				}
				else if (const auto *L = Result.Nodes.getNodeAs<LambdaExpr>(LambdaExprCapturesId))
				{
					annotateLambaPointerCaptures(L);
				}
			}



		} // namespace readability
	} // namespace tidy
} // namespace clang

