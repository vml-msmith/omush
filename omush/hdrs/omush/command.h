/**
 * \file command.h
 *
 * Copyright 2014 Michael Smith
 */

#ifndef OMUSH_HDRS_OMUSH_COMMAND_H_
#define OMUSH_HDRS_OMUSH_COMMAND_H_

#include "omush/command/icommand.h"
#include "omush/command/parser/commandparser.h"
#include "omush/command/matcher/commandmatcherabsolute.h"
#include "omush/command/matcher/commandmatcherpattern.h"
#include "omush/command/matcher/commandmatcherexit.h"
#include "omush/command/matcher/commandmatcherattributesetter.h"
#include "omush/command/matcher/commandmatcherpartial.h"
#include "omush/command/matcher/commandmatcheruserdefined.h"

#include "omush/command/commandquit.h"
#include "omush/command/commandconnect.h"
#include "omush/command/commandcreateplayer.h"
#include "omush/command/commandwho.h"

#include "omush/command/commandhuh.h"
#include "omush/command/commandlook.h"
#include "omush/command/commandexamine.h"
#include "omush/command/commandsay.h"
#include "omush/command/commandpose.h"
#include "omush/command/commandgo.h"
#include "omush/command/commandthink.h"
#include "omush/command/commandset.h"
#include "omush/command/commanddig.h"
#include "omush/command/commandcreate.h"
#include "omush/command/commandinventory.h"
#include "omush/command/commandpower.h"
#include "omush/command/commanddivision.h"

#endif  // OMUSH_HDRS_OMUSH_COMMAND_H_
