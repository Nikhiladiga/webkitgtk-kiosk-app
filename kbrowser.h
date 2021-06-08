#ifndef KBROWSER
#define KBROWSER

#include <gtkmm.h>
#include <webkit2/webkit2.h>

class KBrowser : public Gtk::Window
{

public:
    KBrowser();
    ~KBrowser() override;
    
    bool onKeyPress(GdkEventKey *);
    static gboolean reload_timer(gpointer user_data);
    static gboolean on_load_failed(WebKitWebView *webView, WebKitLoadEvent load_event, gchar *failing_uri, GError *error, gpointer user_data);
};

#endif // KBROWSER
