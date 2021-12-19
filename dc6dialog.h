#ifndef DC6DIALOG_H
#define DC6DIALOG_H

#include <vector>

#include <QDialog>
#include <QGraphicsScene>
#include <QString>

#include "dc6.h"

namespace Ui {
class DC6Dialog;
}

class DC6Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit DC6Dialog(const QString& filename, QWidget *parent = nullptr);
    ~DC6Dialog();

private:
    void buildPixmaps() const;
    void displayFrame(std::size_t frame);

private slots:
    void on_framesSlider_sliderMoved(int position);
    void on_directionsSlider_sliderMoved(int position);

    void on_playButton_released();
    void on_paletteButton_released();

private:
    Ui::DC6Dialog *ui;
    DC6 dc6File;
    QGraphicsScene scene;

    mutable std::vector<std::vector<std::uint8_t>> cachedData;
    mutable std::vector<QPixmap> cachedPixmaps;
};

#endif // DC6DIALOG_H
