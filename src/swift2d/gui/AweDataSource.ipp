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
    std::ifstream ifs(Awesomium::ToString(path), std::ios::in | std::ios::ate);

    if (ifs) {
      std::ifstream::pos_type fileSize = ifs.tellg();
      ifs.seekg(0, std::ios::beg);
      std::vector<char> bytes(fileSize);
      ifs.read(&bytes[0], fileSize);
      html_str = std::string(&bytes[0], fileSize);
    } else {
      Logger::LOG_WARNING << "Failed to load resource \"" << path << "\": File not found!" << std::endl;
    }

    SendResponse(request_id, html_str.size(), (unsigned char*)html_str.c_str(),
                 Awesomium::WSLit("text/html"));
  }

};
