#include "MainWindow.h"
#include <iostream>

std::string get_matches(ProtocolSocket &ps) {
    std::vector<uint8_t> buffer(4096, 1);
    ps.receive(buffer);
    std::string matches(reinterpret_cast<const char *>(buffer.data()), buffer.size());
    //std::string matches = "PARTIDA 1\nPARTIDA 2\nPARTIDA 3\n";
    std::cout << "#-----------------PARTIDAS EXISTENTES---------------#\n";
    std::cout << matches;
    std::cout << "#---------------------------------------------------#\n";
    return matches;
}


MainWindow::MainWindow(ProtocolSocket &ps,
                        QWidget *parent) : QMainWindow(parent),
                                          ui(),
                                          ps(ps),
                                          matches(get_matches(this->ps)),
                                          create_view(this->ps),
                                          join_view(this->ps, this->matches),
                                          start_view(this->ps),
                                          wait_view(this->ps),
                                          fixed(false) {
    ui.setupUi(this);
}

bool MainWindow::is_fixed() {
    return this->fixed;
}

void MainWindow::on_createMatchBtn_clicked(){
    uint8_t start = 2;
    this->ps.send(start);
    this->create_view.show();
    this->create_view.exec();
    if(this->create_view.is_created()) {
        this->close();
        this->start_view.show();
        this->start_view.exec();
        fixed = true;
    }
}

void MainWindow::on_joinMatchBtn_clicked() {
    uint8_t start = 1;
    this->ps.send(start);
    this->join_view.show();
    this->join_view.exec();
    if(this->join_view.is_joined()){
        this->close();
        this->wait_view.show();
        this->wait_view.wait_start();
        fixed = true;
    }
}

MainWindow::~MainWindow() {}


