#include "kbrowser.h"
#include <gtkmm/application.h>
#include <webkit2/webkit2.h>
#include "json.hpp"
#include <fstream>

#define PROGRAM_NAME "kbrowser"
#define AUTHORS proper_name("Nikhil Adiga")

using json = nlohmann::json;

extern WebKitWebView *web_view;

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "");

    KBrowser kbrowser;

    json config;
    std::ifstream ifs("settings.json");
    config = json::parse(ifs);

    //Fullscreen
    if (config["kiosk"] == true)
    {
        kbrowser.fullscreen();
    }
    else
    {
        std::string title = config["title"];

        kbrowser.set_default_size(config["width"], config["height"]);
        kbrowser.set_title(title);
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

    //Hardware acceleration
    if (config["hw_accleration"] == true)
    {
        WebKitSettings *hwsettings = webkit_web_view_get_settings(WEBKIT_WEB_VIEW(web_view));
        g_object_set(G_OBJECT(hwsettings), "enable-webgl", TRUE, NULL);
        g_object_set(G_OBJECT(hwsettings), "enable-accelerated-composting", TRUE, NULL);
    }

    //Developer tools
    if (config["dev_tools"] == true)
    {
        WebKitSettings *dtsettings = webkit_web_view_get_settings(WEBKIT_WEB_VIEW(web_view));
        g_object_set(G_OBJECT(dtsettings), "enable-developer-extras", TRUE, NULL);
    }

    std::string url = config["url"];

    Gtk::Widget *three = Glib::wrap(GTK_WIDGET(web_view));

    kbrowser.add(*three);

    webkit_web_view_load_uri(web_view, &url[0]);

    g_signal_connect(web_view, "load-failed", G_CALLBACK(kbrowser.on_load_failed), NULL);

    kbrowser.show_all();

    app->run(kbrowser);

    exit(0);
}
