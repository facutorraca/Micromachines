#include "StartView.h"
#include <client/qt_views/ui_startView.h>
#include <iostream>

StartView::StartView(ProtocolSocket &ps, QWidget *parent) :
    QDialog(parent),
    ps(ps),
    ui() {
    ui.setupUi(this);
}

void StartView::on_startBtn_clicked() {
    std::string welcome_message(100, '\0');
    std::cout << "Press any key to START" << "\n";
    uint8_t start_game = 1;
      ps.send(start_game);

    bool continue_receiving = true;
    ps.receive(welcome_message);

    std::cout << welcome_message;
    this->close();
}

StartView::~StartView() {
}
