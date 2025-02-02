#ifndef TABS_STATUS_COLUMN_RECORD_H
#define TABS_STATUS_COLUMN_RECORD_H

#include "../column_header.h"
#include "../view/status.h"

#include <gtkmm/cellrenderercombo.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeiter.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treemodelfilter.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <map>
#include <memory>

constexpr int MIN_COL_WIDTH = 20;

class StatusColumnRecord : public Gtk::TreeModel::ColumnRecord
{
public:
  /**
   * @brief Create a new StatusColumnRecord object.
   *
   * @details
   * This method creates a new StatusColumnRecord and an associated TreeStore object.
   * It then sets the TreeStore object as the model for the `view` argument (if `view` already has an underlying model, this will replace
   * that model). Then columns are created for each element of `names` with titles from that std::vector.
   *
   * @param view, The TreeView object to be backed by the new StatusColumnRecord object
   * @param names, The vector of names which will be the titles of columns in the StatusColumnRecord object.
   *
   * @returns std::shared_ptr refrencing a new StatusColumnRecord object.
   */
  static std::shared_ptr<StatusColumnRecord> create(const std::shared_ptr<Gtk::TreeView> &view, const std::vector<ColumnHeader> &names);

  /**
   * @brief Sets the callback function which specifies whether a row should be visible
   *
   * @details
   * Sets the callback function which specifies whether a row should be visible.
   * Function should return true if row should be visible, or false otherwise.
   * This is used do decide which rows should be visible.
   *
   * @param filter, The callback function to use
   */
  void set_visible_func(const Gtk::TreeModelFilter::SlotVisible &filter);

  /**
   * @brief Callback function which for when a user toggles a button, or changes a combobox.
   *
   * @details
   * This callback function takes a string as an argument, which corresponds to the Gtk::Path of the row affected
   *
   * @param path the path pointing to the affected row in the Gtk::TreeView
   */
  typedef sigc::slot<void, const std::string &> change_function_type;

  /**
   * @brief Sets the callback function which is called whenever a row is edited by the user
   *
   * @details
   * Sets the callback function which is called whenever a row is edited by the user.
   * This function is called whevener a user toggles a button, or edited a ComboBox.
   * Specifically, this function connects to the signal_edited() function in Gtk::CellRendererCombo
   * and the signal_toggled() function in Gtk::CellRendererToggle.
   *
   * @param fun the callback function to use
   */
  void set_change_func(const change_function_type &fun);

  /**
   * @brief Creates and returns a new TreeRow in the table.
   *
   * @details
   * Data of the row can be modified using the `set_row_data()` method
   *
   * @returns A new TreeRow from the table.
   */
  Gtk::TreeRow new_row();

  /**
   * @brief Creates and returns a new TreeRow in the table which is the child of another TreeRow.
   *
   * @details
   * Data of the row can be modified using the `set_row_data()` method
   *
   * @param parent, The TreeRow to be the parent of the created TreeRow
   *
   * @returns A new TreeRow from the table.
   */
  Gtk::TreeRow new_child_row(const Gtk::TreeRow &parent);

  /**
   * @brief Removes all rows from the table.
   *
   * @details
   * Removes all rows from the underlying TreeStore, emptying the table.
   */
  void clear();

  /**
   * @brief Gets the TreeRow in the table which is the child of another TreeRow.
   *
   * @details
   * Data of the row can be modified using the `set_row_data()` method
   *
   * @param parent, The TreeRow to be the parent of the created TreeRow
   *
   * @returns A new TreeRow from the table.
   */
  Gtk::TreeRow get_row(const Gtk::TreePath &path);
  Gtk::TreeRow get_row(const Glib::ustring &path);

  Gtk::TreeModel::iterator get_iter(const Gtk::TreePath &path);
  Gtk::TreeModel::iterator get_iter(const Glib::ustring &path);

  /**
   * @brief Set the visibility all the rows in the ColumnRecord.
   *
   * @details
   * Sets the visibility of all the rows in the column record based on the function parameter
   * of `set_visible_function`. If no valid function was specified through that method, before calling this
   * method, then this will have undefined behavior.
   *
   * @returns The number of visible rows.
   */
  uint filter_rows();

  Gtk::TreeRow get_parent_by_pid(unsigned int pid);
  Gtk::TreeRow get_parent_by_pid(unsigned int pid, const Gtk::TreeRow &parent);
  bool pid_exists_in_child(unsigned int pid, const Gtk::TreeRow &parent);

protected:
  // Used by unit-test, the create() method should be used in production code
  StatusColumnRecord(const std::shared_ptr<Status> &tab, const std::vector<ColumnHeader> &names);

  // Called during the create() method, does not initialize certain fields
  explicit StatusColumnRecord(const std::shared_ptr<Gtk::TreeView> &view, const std::vector<ColumnHeader> &names);

private:
  struct RowData
  {
    const bool isSelected;
    const bool isExpanded;

    RowData(const bool &isSelectedArg, const bool &isExpandedArg)
      : isSelected{ isSelectedArg },
        isExpanded{ isExpandedArg }
    {
    }
  };

  Glib::RefPtr<Gtk::TreeStore> store;
  std::shared_ptr<Gtk::TreeView> view;

  Glib::RefPtr<Gtk::TreeModelFilter> filter_model;
  Glib::RefPtr<Gtk::TreeModelSort> sort_model;
  Gtk::TreeModelFilter::SlotVisible filter_fun;
  change_function_type change_fun;

  // Unless `set_visible_func` is called, this filter sets every row in the ColumnRecord to be visible when filtered
  static bool default_filter(const Gtk::TreeModel::iterator &node);

  // Unless `set_change_func` is called, this function does nothing
  static void on_change(const std::string &node);

  // Handles changing a combobox value in a row
  void on_combobox_edited(const Glib::ustring &path_string, const Glib::ustring &new_text, int col);

  // Ignores the parameter and returns an empty string
  // This is used by the cell renderer of columns we do not want to view
  static void ignore_cell_render(Gtk::CellRenderer *renderer, const Gtk::TreeIter &iter);
};

#endif // TABS_STATUS_COLUMN_RECORD_H