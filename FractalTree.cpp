#include "FractalTree.h"
#include "ui_FractalTree.h"

FractalTree::FractalTree(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FractalTree)
{
    ui->setupUi(this);
    if (!ui->centralWidget->layout())
        ui->centralWidget->setLayout(new QGridLayout);
    render();

    connect(ui->renderButton, SIGNAL(clicked()), this, SLOT(render()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save()));
}

FractalTree::~FractalTree()
{
    delete ui;
}

void FractalTree::render() {

    QLabel *label = ui->treeLabel;
    curTree = new FractalTreeImage(ui->widthBox->value(), ui->heightBox->value(), ui->branchesBox->value(), ui->depthBox->value(), ui->rootBox->value());

    //set label background
    QImage background(500, 500, QImage::Format_ARGB32);

    //resize and crop for label
    QImage resized;
    if (curTree->width() > curTree->height())
        resized = curTree->scaledToHeight(500);
    else
        resized = curTree->scaledToWidth(500);

    QRect rect(resized.width() / 2 - 250, resized.height() / 2 - 250, 500, 500);

    QPainter p(&background);
    p.fillRect(0, 0, 500, 500, Qt::white);
    p.drawImage(background.rect(), resized.copy(rect));
    label->setPixmap(QPixmap::fromImage(background));
}

void FractalTree::save() {
    QString filename = QFileDialog::getSaveFileName(this, "save tree", "", "png (*.png);; xpm(*.xpm);; jpg(*.jpg)");
    cout << filename.toStdString() << endl;
    if (filename.endsWith("png"))
        curTree->save(filename, "png", Qt::CaseInsensitive);
    else if (filename.endsWith("jpg", Qt::CaseInsensitive) || filename.endsWith("xpm", Qt::CaseInsensitive)) {
        QImage withBg(curTree->width(), curTree->height(), QImage::Format_RGB32);
        withBg.fill(Qt::white);
        QPainter painter(&withBg);
        painter.drawImage(withBg.rect(), *curTree);
        withBg.save(filename);
    }
}
