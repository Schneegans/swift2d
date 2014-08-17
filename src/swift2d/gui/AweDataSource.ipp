////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

class AweDataSource : public Awesomium::DataSource {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------ public methods
  void OnRequest(int request_id, Awesomium::ResourceRequest const& request,
                 Awesomium::WebString const& path) {

    std::string html_str("<h1>Failed to load resource.</h1>");

    std::string absolute_path(Awesomium::ToString(path));

    // make absolute
    if (absolute_path[0] != '/') {
      absolute_path = Paths::get().make_absolute(absolute_path);
    }

    // strip parameters
    int index = absolute_path.find("?");
    if (index != std::string::npos) {
      absolute_path = absolute_path.substr(0, index);
    }

    TextFile file(absolute_path);

    if (file.is_valid()) {
      html_str = file.get_content();
    } else {
      Logger::LOG_WARNING << "Failed to load resource \"" << path << "\": File not found!" << std::endl;
    }

    SendResponse(request_id, html_str.size(), (unsigned char*)html_str.c_str(),
                 Awesomium::WSLit("text/html"));
  }

};
