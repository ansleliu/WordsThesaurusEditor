#include "StandardDialog.h"
#include "ui_StandardDialog.h"

StandardDialog::StandardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StandardDialog)
{
    ui->setupUi(this);
    todie = "";
    todas = "";
    toder = "";
    moveadj = "";
    moveadv = "";
    movevt = "";
    movevi = "";
    movekonj = "";
    movepron = "";
}

StandardDialog::~StandardDialog()
{
    delete ui;
}

void StandardDialog::on_AcceptButton_clicked()
{
    todie = ui->DieEdit->text();
    todas = ui->DasEdit->text();
    toder = ui->DerEdit->text();
    moveadj = ui->AdjEdit->text();
    moveadv = ui->AdvEdit->text();
    movevt = ui->VtEdit->text();
    movevi = ui->ViEdit->text();
    movekonj = ui->KonjEdit->text();
    movepron = ui->PronEdit->text();
    accept();
}

void StandardDialog::on_QuitButton_clicked()
{
    reject();
}

QStringList StandardDialog::getStandardItems()
{
    QStringList standardItems;
    standardItems << todie
                  << todas
                  << toder
                  << moveadj
                  << moveadv
                  << movevt
                  << movevi
                  << movekonj
                  << movepron;
    return standardItems;
}
