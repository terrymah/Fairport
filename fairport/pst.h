//! \mainpage
//! \author Terry Mahaffey
//! 
//! \section main_intro Introduction
//! This is the API Reference for Fairport. Fairport is a fork of PSTSDK, which
//! is Microsoft's SDK for accessing PST files. 
//!
//! The latest release can be found <a href="latest.zip">here</a>.
//!
//! PSTSDK is distributed under the <a href="http://www.apache.org/licenses">
//! Apache v2.0 License</a>. Fairport is based on PSTSDK, but any and all 
//! Fairport modifications to are licensed under the 
//! <a href="http://www.gnu.org/licenses/gpl.txt">GPL v3</a>. If you require 
//! an Apache licensed library, you can either stick with PSTSDK proper or 
//! contact me directly about alternative licensing for Fairport.
//!
//! Differences between Fairport and PSTSDK:
//!		- Fairport is not associated with Microsoft, PSTSDK is
//!		- Fairport is GPL, PSTSDK is Apache
//!		- Fairport drops all pretense of write support
//!		- Fairport plans on a richer API set at the PST layer
//!
//! Like PSTSDK, Fairport is organized into layers (represented as "Modules" in 
//! this reference). If you don't know where to start, there is a good chance you
//! can work in the "PST" Layer and ignore everything else.
//!
//! \section main_requirements System Requirements
//! - Boost v1.42 or greater (older versions may work, but are untested). 
//! The following Boost components are used:
//!      - Boost.Iostreams
//!      - Boost.Iterators
//!      - Boost.Utility
//! - libiconv (non-Windows)
//! - GCC 4.4.x+ (Linux, Mac, Windows), VC9+ (Windows)
//!
//! \section main_getting_help Additional Documentation
//! See the original PSTSDK Documentation on the 
//! <a href="http://pstsdk.codeplex.com">CodePlex site</a> for a series of 
//! "Quick Start" guides; one for each layer. It's still accurate for 
//! Fairport. 
//!
//! The <a href="https://github.com/terrymah/Fairport/wiki">Fairport Wiki</a>
//! will eventually contain information specific to Fairport.
//!
//! The [MS-PST] reference documentation is included in this distribution 
//! in the "doc" directory, but it may be out of date. The latest version can
//! be found on MSDN.
//!
//! \section main_samples Sample Code
//! There are several sample included in the "samples" directory of the 
//! distribution. Additionally, the unit test code contained in the "test" 
//! directory may be of some value as sample code.
//!
//! \section main_help Getting Help
//! For general PSTSDK questions, you can try the discussion section on the 
//! <a href="http://pstsdk.codeplex.com">CodePlex site</a>. You can also
//! try any other official support mechanism Microsoft may offer.
//!
//! There is currently no forum for Fairport specific questions. You can 
//! try contacting me directly if you'd like to talk about me providing 
//! technical support, or if you feel you've found a bug or have a feature
//! request you can file an issue.
//!
//! \section main_bugs Bug Reports
//! If you find a bug either in the SDK or the documentation, create a work 
//! item for it on the Issue Tracker section of the 
//! <a href="http://pstsdk.codeplex.com">CodePlex site</a>. Feel free to vote
//! up existing issues you feel are important.
//!
//! Please use the <a href="https://github.com/terrymah/Fairport/issues">
//! Issues</a> page for Fairport specific issues or feature requests.
//!
//! \section main_contributions Contributing
//! All contributions (in the form of code contributed back, pull requests,
//! etc) must be Apache licensed or similar - despite Fairport itself being GPL
//! licensed currently. I require this so I have the option of licensing
//! Fairport itself whole via Apache (or similar) in the future. Perhaps it's
//! a silly idea. We'll see how long it lasts.
//!
//! In the meantime, you are of course (and in fact, required) to publish and
//! share any modifications you make under GPLv3, but I most likely will
//! not merge them into Fairport proper.
//!
//! \defgroup pst PST Layer
#ifndef FAIRPORT_PST_H
#define FAIRPORT_PST_H

#include "fairport/pst/pst.h"
#include "fairport/pst/folder.h"
#include "fairport/pst/message.h"

#endif
