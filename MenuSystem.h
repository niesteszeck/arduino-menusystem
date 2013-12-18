/*
  MenuSystem.h - Library for creating menu structures.
  Created by Jon Black, August 8th 2011.
  Released into the public domain.

  License: LGPL 3
*/

#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif


#define MAX_MENU_ITEMS 5
#define MENU_MAXVALUESTRING 8


class MenuComponent
{
public:
    MenuComponent(char* name);
    void set_name(char* name);

    virtual char* get_name() const;
    virtual void set_parent(MenuComponent* pParent);
    virtual boolean next(boolean loop=false);
    virtual boolean prev(boolean loop=false);
    virtual boolean increment(int n=1);
    virtual boolean decrement(int n=1);

    virtual MenuComponent* select() = 0;

protected:
    char* _name;
};


class MenuItem : public MenuComponent
{
public:
    MenuItem(char* name);
    MenuItem(char* name, void (*on_select)(MenuItem*));

    void set_select_function(void (*on_select)(MenuItem*));

    virtual MenuComponent* select();

private:
    void (*_on_select)(MenuItem*);
};

class SetupItem : public MenuComponent
{
public:
    SetupItem(char* name);
    SetupItem(char* name, int value);
    ~SetupItem();
    int get_value();

    virtual boolean increment(int n=1);
    virtual boolean decrement(int n=1);

    virtual void set_name(char* name);
    virtual char* get_name() const;

    virtual MenuComponent* select();

private:
    int _value;
    char* _valueName; //TODO plan a better way to handle the sizze
};

class Menu : public MenuComponent
{
public:
    Menu(char* name);

    virtual boolean next(boolean loop=false);
    virtual boolean prev(boolean loop=false);
    virtual boolean increment(int n=1);
    virtual boolean decrement(int n=1);

    virtual MenuComponent* activate();
    virtual MenuComponent* select();

    virtual MenuComponent const* add(MenuComponent* pMenu);

    virtual void set_parent(Menu* pParent);
    Menu const* get_parent() const;

    MenuComponent const* get_selected() const;
    MenuComponent const* get_menu_component(byte index) const;

    byte get_num_menu_components() const;
    byte get_cur_menu_component_num() const;

private:
    MenuComponent* _menu_components[MAX_MENU_ITEMS];
    Menu* _p_parent;
    byte _num_menu_components;
    byte _cur_menu_component_num;

protected:
    MenuComponent* _p_sel_menu_component;

};
class MenuSystem
{
public:
    MenuSystem();

    boolean next(boolean loop=false);
    boolean prev(boolean loop=false);
    boolean increment(int n=1);
    boolean decerment(int n=1);
    void select(boolean return_to_root=true);
    boolean back();

    void set_root_menu(Menu* p_root_menu);

    Menu const* get_current_menu() const;

private:
    Menu* _p_root_menu;
    Menu* _p_curr_menu;
};


#endif
