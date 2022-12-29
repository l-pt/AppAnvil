#include "profile_modify.h"
#include "../../threads/command_caller.h"
#include "switch_box.h"
#include "switch_row.h"

#include <apparmor_parser.hh>
#include <fstream>
#include <gtkmm/label.h>
#include <memory>
#include <pangomm/attributes.h>
#include <pangomm/attrlist.h>
#include <pangomm/fontdescription.h>
#include <pangomm/layout.h>

ProfileModify::ProfileModify(const std::string &profile_name)
  : m_box{ new Gtk::VBox() }
{
  //// Set values for the "header" widget ////
  header.set_label("Modify Profile");

  // Set the attributes for the "header" wiget
  Pango::AttrList attr;

  // Scale the header by 110%
  constexpr double HEADER_SCALE = 1.1;
  auto scale = Pango::Attribute::create_attr_scale(HEADER_SCALE);
  attr.insert(scale);

  // Make the header bold
  auto weight = Pango::Attribute::create_attr_weight(Pango::WEIGHT_BOLD);
  attr.insert(weight);

  header.set_attributes(attr);

  // Add the header to the main box
  m_box->add(header);

  //// Create the switch boxes ////
  // TODO(error-handling): There might be parsing errors
  std::string profile_location = CommandCaller::locate_profile(profile_name) + profile_name;
  std::fstream profile_stream(profile_location);
  AppArmor::Parser parse(profile_stream);

  auto profile_list = parse.getProfileList();
  if(profile_list.size() == 1) {
    auto abstraction_set = profile_list.front().getAbstractions();

    std::vector<std::string> abstractions = CommandCaller::get_abstractions();
    std::vector<string_tuple> abstraction_vector;

    for(std::string &abstraction : abstractions) {
      const std::string &title    = abstraction;
      const std::string &subtitle = "";
      bool enabled = abstraction_set.contains("abstractions/" + abstraction); 

      abstraction_vector.emplace_back(title, subtitle, enabled);
    }

    std::shared_ptr<SwitchBox> abstraction_box(new SwitchBox(abstraction_vector));
    switch_box_list.push_back(abstraction_box);
    m_box->add(*abstraction_box);
  }

  m_box->set_halign(Gtk::ALIGN_START);
  m_box->set_valign(Gtk::ALIGN_START);

  this->add(*m_box);

  this->show_all();
}
