#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "GraphicsRectItem.h"
#include "HuffmanTree.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QMessageBox>
#include <iostream>
#include <vector>
#include <string>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_scene(new QGraphicsScene(this))
{
    ui->setupUi(this);

    ui->stringEdit->setText("ehal greka cerez reku");

    ui->button_deleteNode->setEnabled(false);
    ui->decodeButton->setEnabled(false);
    ui->graphicsView->setScene(m_scene);


    QObject::connect(ui->button_addNode, &QPushButton::clicked, this, &MainWindow::on_clicked_addNode);
    QObject::connect(ui->button_deleteNode, &QPushButton::clicked, this, &MainWindow::on_clicked_deleteNode);
    QObject::connect(ui->decodeButton, &QPushButton::clicked, this, &MainWindow::on_clicked_decode);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_clicked_addNode()
{
    QString str = ui->stringEdit->text();
    if(str != "")
    {


        std::string code;
        tree.encode(str.toStdString(),code);
        ui->codeEdit->setText(QString::fromStdString(code));
        ui->stringEdit->setText("");
        ui->button_deleteNode->setEnabled(true);
        ui->decodeButton->setEnabled(true);
        onTreeUpdate();
    }
    else
        displayError("line of sring is empty");

}


void MainWindow::on_clicked_deleteNode()
{

    tree.clear();
    ui->button_deleteNode->setEnabled(false);
    ui->decodeButton->setEnabled(false);
    ui->button_addNode->setEnabled(true);
    m_scene->clear();

}


void MainWindow::on_clicked_decode()
{
    QString code = ui->codeEdit->text();
    if(code != "")
    {
        std::string str;
        tree.decode(code.toStdString(),str);
        ui->stringEdit->setText(QString::fromStdString(str));
    }
    else
        displayError("line of code is empty");
}


void MainWindow::onTreeUpdate()
{
    tree.printTree();


       printTree();

}

void MainWindow::displayError(QString const& msg)
{
    QMessageBox::warning(this, "error", msg);

}



void MainWindow::printTree()
{
    int posX = 0;
    int posY = 0;
    printEllipse(tree.getRoot(), posX, posY);
}

void MainWindow::printEllipse(Node* nd, int posX, int posY)
{
    int range = getRange(nd);

    GraphicsRectItem *sceneNode = new GraphicsRectItem(QString::number(nd->frequency()) + " " + QString::fromStdString(nd->symbols()));
    sceneNode->setPos(posX, posY);
    sceneNode->setBrush(Qt::white);

    sceneNode->setBackgroundColor(Qt::green);




    m_scene->addItem(sceneNode);

    if(nd->left())
    {
        m_scene->addLine(posX + 4 , posY + 10,  posX + 4 - range, posY + 50 + 10)->setZValue(-1);//рисуем линию до левой ноды

        std::cout << "range: " <<  range << std::endl;
        printEllipse(nd->left(), posX - range, posY + 50 );

    }
    if(nd->right())
    {
        m_scene->addLine(posX + 4 , posY + 10,  posX + 4 + range, posY + 50 + 10)->setZValue(-1);
        std::cout << "range: " <<  range << std::endl;
        printEllipse(nd->right(), posX + range, posY + 50);//рисуем линию до правой ноды

    }

}


int MainWindow::getRange(Node* nd)
{
    int minrange = 10;

    int range = minrange + nodeRadius/2;
    int width = 2 * nodeRadius + 2 * minrange;



    for(int i = 0; i < tree.getHeight(nd) - 2; i++ )//ширина дерева в зависимости от высоты
    {
        width = 2 * width;
        range = width/2 - nodeRadius;
    }


    return range;
}




