#include <QtGui/QtGui>
#include <QApplication>
//
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>

#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QFileDialog>

QImage blurImage(QImage source, int radius)
{
    if(source.isNull()) return QImage();
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(source));

    auto *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(radius);
    item.setGraphicsEffect(blur);
    scene.addItem(&item);
    QImage result(source.size(), QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    scene.render(&painter, QRectF(),
                 QRectF(0,0,source.width(),source.height()));
    return result;
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget blurFlowWindow;
    blurFlowWindow.resize(800, 600);

    QVBoxLayout vbox(&blurFlowWindow);
    //Label - for image
    auto *imageLabel = new QLabel(&blurFlowWindow);
    vbox.addWidget(imageLabel);
    //slider for blur
    auto *blurValueSlider = new QSlider(&blurFlowWindow);
    blurValueSlider->setRange(0, 10);
    blurValueSlider->setValue(0);
    blurValueSlider->setOrientation(Qt::Horizontal);
    vbox.addWidget(blurValueSlider);
    //button for open file directory
    auto *openFileButton = new QPushButton("Open",&blurFlowWindow);
    vbox.addWidget(openFileButton);
    QString filePath;
    QImage originalImage;
    QObject::connect(openFileButton, &QPushButton::clicked, [&originalImage, &blurValueSlider, &filePath, &imageLabel]{
       filePath = QFileDialog::getOpenFileName(nullptr, "Open-jpg","/","Images (*.jpg *.JPG *.jpeg)");
       if (filePath.isEmpty()) return;

       QImage image(filePath);

       if(!image.isNull())
       {
          originalImage = image.scaled(
                               imageLabel->size(),
                               Qt::KeepAspectRatio,
                               Qt::SmoothTransformation);
          imageLabel->setPixmap(QPixmap::fromImage(originalImage));
          blurValueSlider->setValue(0);
          imageLabel->setAlignment(Qt::AlignCenter);
       }
    });

    QObject::connect(blurValueSlider, &QSlider::valueChanged, [&originalImage, &imageLabel](int value){
        if(!originalImage.isNull())
            if(value == 0)
            {
                imageLabel->setPixmap(QPixmap::fromImage(originalImage));
            }
            else
            {
                QImage blured = blurImage(originalImage, value);
                imageLabel->setPixmap(QPixmap::fromImage(blured));
            }
    });

    blurFlowWindow.show();
    return app.exec();
}

#include "main.moc"
