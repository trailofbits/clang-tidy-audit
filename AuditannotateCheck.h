#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_READABILITY_AUDITANNOTATE_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_READABILITY_AUDITANNOTATE_H

#include "../ClangTidy.h"

namespace clang {
	namespace tidy {
		namespace readability {

			class AuditannotateCheck : public ClangTidyCheck {
				public:
					AuditannotateCheck(StringRef Name, ClangTidyContext *Context)
						: ClangTidyCheck(Name, Context) {}

					void registerMatchers(ast_matchers::MatchFinder *Finder) override;
					void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

				private:
					void replaceUsesOfAuto(const VarDecl *V);
					void elaborateOnImplicitCasts(const ImplicitCastExpr *I);
					bool typeHasPointerMembers(const Type *V);
					void annotateLambaPointerCaptures(const LambdaExpr *L);

			};

		} // namespace readability
	} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_READABILITY_AUDITANNOTATE_H
