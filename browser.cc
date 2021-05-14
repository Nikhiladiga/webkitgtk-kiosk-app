/**
 *  Pre-requisites
 *  apt-get install libgtkmm-3.0-dev
 *  apt-get install libgstreamermm-1.0-dev
 *  apt-get install build-essential
 *  Compilation command - g++ browser.cc -o browser `pkg-config gtkmm-3.0 --libs --cflags` `pkg-config webkit2gtk-4.0 --libs --cflags` -std=c++11
**/

#include <gtkmm.h>
#include <webkit2/webkit2.h>
#include <iostream>

static WebKitWebView* web_view;

class BrowserWindow : public Gtk::Window
{
  public:
  BrowserWindow();
  bool onKeyPress(GdkEventKey*);
};

BrowserWindow::BrowserWindow(){
  this->signal_key_press_event().connect( sigc::mem_fun( *this, &BrowserWindow::onKeyPress ),false );
}

bool BrowserWindow::onKeyPress(GdkEventKey* event)
{
    //Check if pressed button's keyval is equal to F5 keyval
    if(event->keyval == 65474){
      webkit_web_view_reload_bypass_cache(web_view);
    }
    return false;
}

int main(int argc, char **argv)
{
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "");

  BrowserWindow window;

  window.fullscreen();

  WebKitAutoplayPolicy autoplay = WEBKIT_AUTOPLAY_ALLOW;
  WebKitWebsitePolicies *wp = webkit_website_policies_new_with_policies("autoplay", autoplay, NULL);

  web_view = WEBKIT_WEB_VIEW(g_object_new(WEBKIT_TYPE_WEB_VIEW, "website-policies", wp, webkit_web_view_new()));

  Gtk::Widget *three = Glib::wrap(GTK_WIDGET(web_view));

  window.add(*three);
  webkit_web_view_load_uri(web_view, "https://threejs.org/");

  window.show_all();

  app->run(window);
  exit(0);
}
