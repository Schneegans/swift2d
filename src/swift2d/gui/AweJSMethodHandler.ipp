////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

class AweJSMethodHandler : public Awesomium::JSMethodHandler {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface
  AweJSMethodHandler(GuiComponent* parent)
    : parent_(parent) {}

  // ------------------------------------------------------------ public methods
  void OnMethodCall(
    Awesomium::WebView* caller, unsigned int remote_object_id,
    const Awesomium::WebString& method_name, const Awesomium::JSArray& args) {

    parent_->on_javascript_callback.emit(Awesomium::ToString(method_name));
  }

  Awesomium::JSValue OnMethodCallWithReturnValue(
    Awesomium::WebView* caller, unsigned int remote_object_id,
    const Awesomium::WebString& method_name, const Awesomium::JSArray& args) {

  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  GuiComponent* parent_;

};
