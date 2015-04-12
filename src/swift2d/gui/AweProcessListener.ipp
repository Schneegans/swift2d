////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

class AweProcessListener : public Awesomium::WebViewListener::Process {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------ public methods
  void OnUnresponsive(Awesomium::WebView* caller) {
    LOG_WARNING << "OnUnresponsive" << std::endl;
  }

  void OnResponsive(Awesomium::WebView* caller) {
    LOG_WARNING << "OnResponsive" << std::endl;
  }

  void OnCrashed(Awesomium::WebView* caller,
                         Awesomium::TerminationStatus status) {
    LOG_WARNING << "OnCrashed" << std::endl;
  }

  void OnLaunch(Awesomium::WebView* caller) {
    //LOG_WARNING << "OnLaunch" << std::endl;
  }
};
