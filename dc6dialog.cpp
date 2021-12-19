#include "dc6dialog.h"
#include "ui_dc6dialog.h"

DC6Dialog::DC6Dialog(const QString &filename, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DC6Dialog),
    dc6File(filename.toStdString())
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);

    ui->directionsSlider->setRange(0, dc6File.getDirections()-1);
    ui->framesSlider->setRange(0, dc6File.getFramesPerDirection()-1);

    on_directionsSlider_sliderMoved(0);

    // TODO: automatically scale to biggest frame of current direction
    ui->graphicsView->scale(4.0, 4.0);
}

DC6Dialog::~DC6Dialog()
{
    delete ui;
}

void DC6Dialog::buildPixmaps() const
{
    cachedData.clear();
    cachedPixmaps.clear();

    cachedData.reserve(dc6File.getFramesPerDirection());
    cachedPixmaps.reserve(dc6File.getFramesPerDirection());

    std::size_t direction = ui->directionsSlider->value();
    for(std::size_t frame = 0; frame < dc6File.getFramesPerDirection(); ++frame)
    {
        auto width = dc6File.getFrameWidth(direction, frame);
        auto height = dc6File.getFrameHeight(direction, frame);

        cachedData.emplace_back(dc6File.renderFrameToARGB(direction, frame));
        cachedPixmaps.emplace_back(QPixmap::fromImage(
                                   QImage(cachedData.at(frame).data(), width, height, QImage::Format_ARGB32)));
    }
}

void DC6Dialog::displayFrame(std::size_t frame)
{
    scene.clear();
    scene.addPixmap(cachedPixmaps.at(frame));
}

void DC6Dialog::on_framesSlider_sliderMoved(int position)
{
    ui->frameLabel->setText(QString("Frame ") + QString::number(position+1) +
                            QString("/") + QString::number(dc6File.getFramesPerDirection()));
    displayFrame(position);
}


void DC6Dialog::on_directionsSlider_sliderMoved(int position)
{
    ui->directionLabel->setText(QString("Direction ") + QString::number(position+1) +
                                QString("/") + QString::number(dc6File.getDirections()));

    buildPixmaps();
    ui->framesSlider->setValue(0);
    on_framesSlider_sliderMoved(0);
}

void DC6Dialog::on_playButton_released()
{

}

void DC6Dialog::on_paletteButton_released()
{

}

