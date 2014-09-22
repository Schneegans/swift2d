////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Downloader.hpp>

#include <swift2d/application/Paths.hpp>

#include <curl/curl.h>
#include <thread>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Downloader::Downloader()
  : ProgressBytes(0)
  , ProgressPercent(0)
  , current_dl_(0)
  , total_dl_(0) {}

////////////////////////////////////////////////////////////////////////////////

void Downloader::download(std::string const& uri) {
  download(uri, Paths::get().tmp_file());
}

////////////////////////////////////////////////////////////////////////////////

void Downloader::download(std::string const& uri, std::string const& result_file) {
  ProgressBytes = 0;
  ProgressPercent = 0;
  current_dl_ = 0;
  total_dl_ = -1;

  result_ = result_file;

  std::thread load_thread([uri, this](){
    CURL *curl;
    FILE *fp;
    CURLcode res;

    char error_buffer[CURL_ERROR_SIZE];
    curl = curl_easy_init();

    if (curl) {
      fp = fopen(result_.c_str(),"wb");
      curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);
      curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Downloader::write_func);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
      curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, Downloader::progress_func);
      curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);
      curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
      res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);
      fclose(fp);

      if (res != CURLE_OK) {
        error_ = std::string(error_buffer);
      }
    }
  });

  load_thread.detach();
}

////////////////////////////////////////////////////////////////////////////////

void Downloader::update() {
  if (total_dl_ > 0) {
    ProgressBytes = current_dl_;
    ProgressPercent = 100.0f * current_dl_ / total_dl_;
  }

  if (error_ != "") {
    on_error.emit(error_);
    error_ = "";
  }
}

////////////////////////////////////////////////////////////////////////////////

size_t Downloader::write_func(void *ptr, size_t size, size_t nmemb, FILE *stream, void* user_data) {
  size_t written;
  written = fwrite(ptr, size, nmemb, stream);
  return written;
}

////////////////////////////////////////////////////////////////////////////////

int Downloader::progress_func(void* s, long total, long now, long, long) {
  static_cast<Downloader*>(s)->current_dl_ = now;
  static_cast<Downloader*>(s)->total_dl_ = total;
  return 0;
}

////////////////////////////////////////////////////////////////////////////////

}
