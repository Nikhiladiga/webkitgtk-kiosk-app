#include <gtkmm.h>
#include <webkit2/webkit2.h>
#include "kbrowser.h"


WebKitWebView *web_view;

KBrowser::KBrowser()
{
  this->signal_key_press_event().connect(sigc::mem_fun(*this, &KBrowser::onKeyPress), false);
}

KBrowser::~KBrowser()
{

}

bool KBrowser::onKeyPress(GdkEventKey *event)
{
  if (event->keyval == 65474)
  {
    webkit_web_view_reload_bypass_cache(web_view);
  }
  return false;
}

gboolean KBrowser::reload_timer(gpointer user_data)
{
  char *failing_uri = (char *)user_data;
  webkit_web_view_load_uri(web_view, failing_uri);
  free(failing_uri);
  return FALSE;
}

gboolean KBrowser::on_load_failed(WebKitWebView *webView, WebKitLoadEvent load_event, gchar *failing_uri, GError *error, gpointer user_data)
{
  fprintf(stderr, "Load failed. Reloading %s\n", failing_uri);
  g_timeout_add(2500 + rand() % 5000, reload_timer, strdup(failing_uri));
  return TRUE;
}