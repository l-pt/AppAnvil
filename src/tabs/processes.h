#ifndef GTKMM_EXAMPLE_PROCESSES_H
#define GTKMM_EXAMPLE_PROCESSES_H

#include "status.h"

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <memory>
#include <string>

class Processes : public Status
{
  public:
    Processes();
    void refresh();

  protected:
    // Signal handlers    
    void on_search_changed();

    const std::vector<std::string> col_names{"Process"};
    std::shared_ptr<StatusColumnRecord> col_record;

};

#endif // GTKMM_EXAMPLE_PROCESSES_H
