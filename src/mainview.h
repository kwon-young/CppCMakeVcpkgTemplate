#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>

namespace Ui {
class MainView;
}

class MainView : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainView(QWidget *parent = 0);
    ~MainView();

private slots:
    void open();
    void saveAs();

private:
    bool loadFile(const QStringList &fileNames);
    bool saveFile(const QString &fileName);
    void setImage(const QImage &newImage);
    Ui::MainView *ui;

    QImage image;
    float scaleFactor;
};

#endif // MAINVIEW_H
