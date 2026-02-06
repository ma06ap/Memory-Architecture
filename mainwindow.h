#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QTextEdit>
#include <QTimer>
#include "Cache.h"
#include "RAM.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void on_simulationTimerTick();

private:
    Ui::MainWindow *ui;

    QTimer *simTimer;

    // پوینتر به حافظه‌ها
    Cache *l1 = nullptr;
    Cache *l2 = nullptr;
    Cache *l3 = nullptr;
    RAM *ram = nullptr;

    // متغیرهای مربوط به دیسک
    uint64_t diskSizeBytes;
    int diskDelayMs;

    // متغیرهای آمارگیری (شمارنده تست‌ها و زمان‌ها)
    uint64_t totalTests;
    uint64_t totalHits;
    double totalLatencyUs;

    uint64_t calculateSize(double value, int unitIndex);
    void simulateMemoryAccess(uint64_t address);
    void toggleSettings(bool enable);

    Replacement getPolicyFromCombo(QComboBox* combo);
    double getSafeDelay(Memory* mem);
};

#endif // MAINWINDOW_H
