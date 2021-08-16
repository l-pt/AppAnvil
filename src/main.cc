#include "resource.autogen.c"
#include "main_window.h"

#include <gtkmm/application.h>

void register_resource_bundle(){
  Glib::RefPtr<Gio::Resource> resource_bundle = Glib::wrap(resources_get_resource());
  resource_bundle->register_global();
}

int main()
{
  auto app = Gtk::Application::create("com.github.jack-ullery");
  register_resource_bundle();

  // Shows the window and returns when it is closed.
  MainWindow win;
  return app->run(win);
}
