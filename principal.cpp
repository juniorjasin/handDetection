#include "principal.h"
#include "ui_principal.h"

principal::principal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::principal)
{
    ui->setupUi(this);


    ui->SceneCamara->showMaximized();

    // valores por defecto para analizar valores de la mano
    ui->SceneCamara->refSkin->addMinHue(90);
    ui->SceneCamara->refSkin->addMaxHue(140);

}

principal::~principal()
{
    delete ui;
}
