/* MyIGS.cpp */

#include <iostream>
#include "MyIGS.h"

MyIGS::MyIGS() :
        _mainBox(Gtk::ORIENTATION_HORIZONTAL),
        _zoomButtonOut("Out"),
        _zoomButtonIn("In"),
        _dispButtonUp("Up"),
        _dispButtonDown("Down"),
        _dispButtonLeft("Left"),
        _dispButtonRight("Right") {

    /* Main window */
    set_title("My IGS");
    set_border_width(10);
    set_default_size(640,480);

    /* Frames */
    MyIGS::createControlFrame();
    MyIGS::createViewportFrame();
    MyIGS::createObjectsFrame();

    add(_mainBox);
    show_all_children();
}

MyIGS::~MyIGS() {
}

void MyIGS::createControlFrame() {
    _controlBox.set_orientation(Gtk::ORIENTATION_VERTICAL);
    _zoomBox.set_orientation(Gtk::ORIENTATION_HORIZONTAL);

    /* Zoom frame */
    _zoomFrame.set_label("Zoom");
    _zoomFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
    _zoomFrame.set_border_width(5);
    _zoomFrame.add(_zoomBox);
    _controlBox.pack_start(_zoomFrame, Gtk::PACK_SHRINK, 1);

    /* Zoom frame buttons */
    _zoomButtonOut.signal_clicked().connect(sigc::mem_fun(*this, &MyIGS::zoom_out));
    _zoomButtonIn.signal_clicked().connect(sigc::mem_fun(*this, &MyIGS::zoom_in));
    _zoomBox.pack_start(_zoomButtonOut, Gtk::PACK_EXPAND_WIDGET, 1);
    _zoomBox.pack_start(_zoomButtonIn, Gtk::PACK_EXPAND_WIDGET, 1);

    /* Displacement Frame */
    _dispFrame.set_label("Displacement");
    _dispFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
    _dispFrame.set_border_width(5);
    _controlBox.pack_start(_dispFrame, Gtk::PACK_SHRINK, 1);

    /* Displacement buttons */
    _dispGrid.attach(_dispButtonUp,2,1,1,1);
    _dispGrid.attach(_dispButtonRight,3,2,1,1);
    _dispGrid.attach(_dispButtonDown,2,3,1,1);
    _dispGrid.attach(_dispButtonLeft,1,2,1,1);
    _dispFrame.add(_dispGrid);

    _controlFrame.set_label("Control");
    _controlFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
    _controlFrame.add(_controlBox);

    _mainBox.pack_start(_controlFrame, Gtk::PACK_SHRINK, 1);
}

void MyIGS::createViewportFrame() {
    _viewportBox.set_orientation(Gtk::ORIENTATION_VERTICAL);
    _viewportBox.pack_start(_canvas, Gtk::PACK_EXPAND_WIDGET, 1);
    _viewportBox.pack_start(_board, Gtk::PACK_EXPAND_WIDGET, 1);

    _viewportFrame.set_label("Viewport");
    _viewportFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);

    _viewportFrame.add(_viewportBox);
    _mainBox.pack_start(_viewportFrame, Gtk::PACK_EXPAND_WIDGET, 1);
}

void MyIGS::createObjectsFrame() {
    _objectsFrame.set_label("Objects");
    _objectsFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
    _mainBox.pack_start(_objectsFrame, Gtk::PACK_EXPAND_WIDGET, 1);
}

void MyIGS::zoom_in() {
    std::cout << "Zooming in!" << std::endl;
}

void MyIGS::zoom_out() {
    std::cout << "Zooming out!" << std::endl;
}

void MyIGS::displaceUp() {

}

void MyIGS::displaceRight() {

}

void MyIGS::displaceDown() {

}

void MyIGS::displaceLeft() {

}