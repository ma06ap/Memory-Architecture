#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <cmath>
#include <random>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    simTimer = new QTimer(this);
    connect(simTimer, &QTimer::timeout, this, &MainWindow::on_simulationTimerTick);
    QStringList units = {"Bytes", "KB", "MB", "GB", "TB"};
    QList<QComboBox*> combos = {
        ui->comboUnitL1, ui->comboUnitL2, ui->comboUnitL3,
        ui->comboUnitRAM, ui->comboUnitDisk
    };
    for(auto c : combos) c->addItems(units);
    ui->comboUnitL1->setCurrentIndex(1);
    ui->comboUnitL2->setCurrentIndex(1);
    ui->comboUnitL3->setCurrentIndex(2);
    ui->comboUnitRAM->setCurrentIndex(3);
    ui->comboUnitDisk->setCurrentIndex(4);
}
MainWindow::~MainWindow()
{
    delete ui;
    delete l1; delete l2; delete l3; delete ram;
}
uint64_t MainWindow::calculateSize(double value, int unitIndex) {
    return (uint64_t)(value * std::pow(1024, unitIndex));
}
Replacement MainWindow::getPolicyFromCombo(QComboBox* combo) {
    QString txt = combo->currentText();
    if(txt == "LRU") return LRU;
    if(txt == "FIFO") return FIFO;
    return RR;
}
double MainWindow::getSafeDelay(Memory* mem) {
    if(!mem) return 0.0;
    int d = mem->getDelay();
    return (d < 0) ? 0.1 : (double)d;
}
void MainWindow::on_btnStart_clicked()
{
    try {
        totalTests = 0; totalHits = 0; totalLatencyUs = 0;
        ui->txtLog->clear();
        uint64_t l1Size = calculateSize(ui->spinSizeL1->value(), ui->comboUnitL1->currentIndex());
        l1 = new Cache(l1Size, ui->spinDelayL1->value(), ui->spinBlockL1->value(), 2, getPolicyFromCombo(ui->comboPolicyL1));
        if(ui->groupL2->isChecked()) {
            uint64_t l2Size = calculateSize(ui->spinSizeL2->value(), ui->comboUnitL2->currentIndex());
            l2 = new Cache(l2Size, ui->spinDelayL2->value(), ui->spinBlockL2->value(), 4, getPolicyFromCombo(ui->comboPolicyL2));
        }
        if(ui->groupL3->isChecked()) {
            uint64_t l3Size = calculateSize(ui->spinSizeL3->value(), ui->comboUnitL3->currentIndex());
            l3 = new Cache(l3Size, ui->spinDelayL3->value(), ui->spinBlockL3->value(), 8, getPolicyFromCombo(ui->comboPolicyL3));
        }
        uint64_t ramSize = calculateSize(ui->spinSizeRAM->value(), ui->comboUnitRAM->currentIndex());
        ram = new RAM(ramSize, ui->spinDelayRAM->value() * 1000, 4096, LRU);
        diskSizeBytes = calculateSize(ui->spinSizeDisk->value(), ui->comboUnitDisk->currentIndex());
        diskDelayMs = ui->spinDelayDisk->value();
        toggleSettings(false);
        ui->btnStart->setEnabled(false);
        ui->btnStop->setEnabled(true);
        simTimer->start(1);
        ui->txtLog->append("<b>--- Simulation Started (Burst Traffic Mode) ---</b>");

    } catch (std::exception &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void MainWindow::on_btnStop_clicked()
{
    simTimer->stop();
    double avgMs = (totalTests > 0) ? (totalLatencyUs / totalTests / 1000.0) : 0;
    QString msg = QString("Tests: %1\nHits: %2\nHit Rate: %3%\nAvg Time: %4 ms")
                      .arg(totalTests).arg(totalHits)
                      .arg((double)totalHits/totalTests*100.0, 0, 'f', 2)
                      .arg(avgMs, 0, 'f', 4);
    QMessageBox::information(this, "Results", msg);
    ui->txtLog->append("<br><b>" + msg.replace("\n", "<br>") + "</b>");
    delete l1; l1=nullptr; delete l2; l2=nullptr; delete l3; l3=nullptr; delete ram; ram=nullptr;
    ui->btnStart->setEnabled(true); ui->btnStop->setEnabled(false); toggleSettings(true);
}
void MainWindow::on_simulationTimerTick()
{
    static uint64_t currentBaseAddress = 0;
    static int remainingInBurst = 0;
    static std::mt19937_64 rng(std::random_device{}());
    for(int k=0; k<10; k++) {
        if (remainingInBurst <= 0) {
            std::uniform_int_distribution<uint64_t> distAddr(0, diskSizeBytes - 100);
            std::uniform_int_distribution<int> distCount(10, 20);

            currentBaseAddress = distAddr(rng);
            remainingInBurst = distCount(rng);
        }
        currentBaseAddress += 4;
        remainingInBurst--;
        simulateMemoryAccess(currentBaseAddress);
    }
}
void MainWindow::simulateMemoryAccess(uint64_t address)
{
    if(!l1) return;
    totalTests++;
    double delayUs = 0;
    bool hit = false;
    QString status = "DISK";
    delayUs += getSafeDelay(l1);
    if(l1->getData(address)) {
        status = "L1 HIT"; hit = true;
    } else {
        if(l2) {
            delayUs += getSafeDelay(l2);
            if(l2->getData(address)) {
                status = "L2 HIT"; hit = true;
                l1->addBlock(address);
            }
        }
        if(!hit && l3) {
            delayUs += getSafeDelay(l3);
            if(l3->getData(address)) {
                status = "L3 HIT"; hit = true;
                if(l2) l2->addBlock(address);
                l1->addBlock(address);
            }
        }
        if(!hit) {
            delayUs += getSafeDelay(ram);
            if(ram->getData(address)) {
                status = "RAM HIT"; hit = false;
                if(l3) l3->addBlock(address);
                if(l2) l2->addBlock(address);
                l1->addBlock(address);
            } else {
                delayUs += (diskDelayMs * 1000.0);
                ram->addPage(address);
                if(l3) l3->addBlock(address);
                if(l2) l2->addBlock(address);
                l1->addBlock(address);
            }
        }
    }
    if(hit) totalHits++;
    totalLatencyUs += delayUs;
    if(totalTests % 10 == 0) {
        ui->txtLog->append(QString("0x%1 | %2 | %3 ms")
                               .arg(QString::number(address, 16).toUpper())
                               .arg(status)
                               .arg(delayUs/1000.0, 0, 'f', 3));
    }
}
void MainWindow::toggleSettings(bool enable) {
    ui->groupL1->setEnabled(enable); ui->groupL2->setEnabled(enable);
    ui->groupL3->setEnabled(enable); ui->groupRAM->setEnabled(enable);
    ui->groupDisk->setEnabled(enable);
}
