/**
 *
 */

#include "omush/command/icommand.h"
#include "omush/notifier.h"

namespace omush {
  void ICommand:: notifyExecutor(CommandContext& context, std::string msg) {
    Notifier(*(context.game), *(context.db)).notify(context.cmdScope.enactor,
                                                    msg);
  }
}
