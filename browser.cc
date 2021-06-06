/**
 *  Pre-requisites
 *  apt-get install libgtkmm-3.0-dev
 *  apt-get install libgstreamermm-1.0-dev
 *  apt-get install build-essential
 *  Compilation command - g++ browser.cc -o browser `pkg-config gtkmm-3.0 --libs --cflags` `pkg-config webkit2gtk-4.0 --libs --cflags` -std=c++11
**/

#include <gtkmm.h>
#include <webkit2/webkit2.h>
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <vector>

using json = nlohmann::json;

static WebKitWebView *web_view;

class BrowserWindow : public Gtk::Window
{
public:
  BrowserWindow();
  bool onKeyPress(GdkEventKey *);
};

BrowserWindow::BrowserWindow()
{
  this->signal_key_press_event().connect(sigc::mem_fun(*this, &BrowserWindow::onKeyPress), false);
}

bool BrowserWindow::onKeyPress(GdkEventKey *event)
{
  if (event->keyval == 65474)
  {
    webkit_web_view_reload_bypass_cache(web_view);
  }
  return false;
}

static gboolean reload_timer(gpointer user_data)
{
  char *failing_uri = (char *)user_data;
  webkit_web_view_load_uri(web_view, failing_uri);
  free(failing_uri);
  return FALSE;
}

static gboolean on_load_failed(WebKitWebView *webView, WebKitLoadEvent load_event, gchar *failing_uri, GError *error, gpointer user_data)
{
  fprintf(stderr, "Load failed. Reloading %s\n", failing_uri);
  g_timeout_add(2500 + rand() % 5000, reload_timer, strdup(failing_uri));
  return TRUE;
}

std::ifstream getFileContent(std::string fileName)
{
  std::ifstream ifs(fileName);
  return ifs;
}

int main(int argc, char **argv)
{
  auto app = Gtk::Application::create(argc, argv, "");

  BrowserWindow window;

  json config;
  config = json::parse(getFileContent("settings.json"));

  //Fullscreen
  if (config["kiosk"] == true)
  {
    window.fullscreen();
  }
  else
  {
    std::string title = config["title"];

    window.set_default_size(config["width"], config["height"]);
    window.set_title(title);
  }

  //Audio Autoplay
  if (config["autoplay"] == true)
  {
    WebKitAutoplayPolicy autoplay = WEBKIT_AUTOPLAY_ALLOW;
    WebKitWebsitePolicies *wp = webkit_website_policies_new_with_policies("autoplay", autoplay, NULL);
    web_view = WEBKIT_WEB_VIEW(g_object_new(WEBKIT_TYPE_WEB_VIEW, "website-policies", wp, webkit_web_view_new()));
  }
  else
  {
    web_view = WEBKIT_WEB_VIEW(webkit_web_view_new());
  }

  std::string url = config["url"];

  Gtk::Widget *three = Glib::wrap(GTK_WIDGET(web_view));

  window.add(*three);

  webkit_web_view_load_uri(web_view, &url[0]);

  g_signal_connect(web_view, "load-failed", G_CALLBACK(on_load_failed), NULL);

  window.show_all();

  app->run(window);

  exit(0);
}
