#pragma once

#include "ui_CactusReaderForPDF.h"
#include "SimpleApp.h"

class CactusReaderForPDF : public QMainWindow
{
    Q_OBJECT

public:
    CactusReaderForPDF(QWidget *parent = nullptr);

private:
    void resizeEvent(QResizeEvent* event) override;

private:
    Ui::CactusReaderForPDFClass ui;
};