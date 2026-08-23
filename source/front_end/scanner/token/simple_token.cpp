#include "simple_token.hpp"
#include "comment_token.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
void SimpleToken::setCommentParent(CommentToken* comment) {
    while (comment != nullptr) {
        comment->parent = this;
        comment = dynamic_cast<CommentToken*>(comment->getNext());  // TODO: Rewrite to avoid casting
    }
}
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart