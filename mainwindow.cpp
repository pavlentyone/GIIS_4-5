#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand(0);

    aboutWindow = new About(this);

    fifteenWidth = 6;
    fifteenHeight = 3;

    const QPalette palet(QPalette::Background, QColor(150,100,250,255));
    this->centralWidget()->setPalette(palet);
    this->centralWidget()->setAutoFillBackground(true);
    this->statusBar()->setPalette(palet);
    this->statusBar()->setAutoFillBackground(true);


    //create mass with order of fifteen's blocks
    blockPosition.resize(fifteenWidth * fifteenHeight);
    for(int i = 0; i < blockPosition.length(); i++)
        blockPosition[i] = i;

    filePath = "/home/pavlenty/QtProgects/GIIT_1/img/catgirl.jpg";

    //create vector of fifteen's blocks(labels)
    labels.resize(fifteenWidth * fifteenHeight);
    QGridLayout *mainGridLayout = new QGridLayout;
    for(int i = 0 ; i < fifteenHeight; i++)
        for(int j = 0; j < fifteenWidth; j++){
            labels[fifteenWidth * i + j] = new QLabel(QString("l%1").arg(fifteenWidth * i + j));
            mainGridLayout->addWidget(labels[fifteenWidth * i + j], i, j);
        }
    ui->centralWidget->setLayout(mainGridLayout);

    //connections
    connect(ui->actionShake, &QAction::triggered,
            this,            &MainWindow::on_actionShake_triggered);
    connect(ui->actionLoadImage, &QAction::triggered,
            this,                &MainWindow::on_actionLoadImage_triggered);
    connect(ui->actionOrder, &QAction::triggered,
            this,            &MainWindow::on_actionOrder_triggered);
    connect(ui->actionStandartBlocks, &QAction::triggered,
            this,                     &MainWindow::on_actionStandartBlocks_triggered);
    connect(ui->actionLoadBackground, &QAction::triggered,
            this,                     &MainWindow::on_actionLoadBackground_triggered);
    connect(ui->actionStandartBackground, &QAction::triggered,
            this,                         &MainWindow::on_actionStandartBackground_triggered);
    connect(ui->menuAbout, &QMenu::triggered,
            this,          &MainWindow::on_actionAbout_triggered);


    for(int i = 0; i < labels.length(); i++){
        labels[i]->setScaledContents(true);
        labels[i]->installEventFilter(this);
        labels[i]->setAlignment(Qt::AlignCenter);
    }
    poorBlockNum = blockPosition.length() - 1;

    isWin = false;
    isBackGroundNow = false;
    isImageNow = false;
    for(int i = 0; i < labels.length(); i++){
        labels[i]->setAutoFillBackground(true);
    }

    fillBlocks();

    myTimer.start();
    dialogWindow = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;

    delete aboutWindow;
    if(dialogWindow != NULL)
        delete dialogWindow;
}

void MainWindow::on_actionShake_triggered(){
    QMessageBox::about(this, "message", "Good luck!");

    //initialize boolean mass to shake the fifteen's blocks
    QVector<bool> blockPositionCheck;
    blockPositionCheck.resize(blockPosition.length());
    for(int i = 0 ; i < blockPositionCheck.length(); i++)
        blockPositionCheck[i] = true;

    //shake positions of fifteen's blocks
    for(int i = 0; i < blockPosition.length(); i++){
        //random generated index of positive values
        int gen = rand() % blockPositionCheck.length();
        //value of current index
        int thisPos = -1;
        //counter of positive values
        int j = 0;
        //searching of gen-index positive value
        while(j <= gen){
            //increace value before use it
            thisPos++;
            //if we go out of limit so continue from the start
            thisPos %= blockPositionCheck.length();
            //if current index is positive so increace the counter
            if(blockPositionCheck[thisPos])
                j++;
        }
        //after we have searched random generated index of positive values
        //so make it negative and create one more randpm value in mass we need
        blockPositionCheck[thisPos] = false;
        blockPosition[thisPos] = i;
    }

    for(int i = 0; i < blockPosition.length(); i++)
        if(blockPosition[i] == blockPosition.length() - 1){
            poorBlockNum = i;
            break;
        }

    ui->centralWidget->layout()->setSpacing(5);

    isWin = false;
    //shake the blocks
    fillBlocks();
    myTimer.restart();
}

void MainWindow::on_actionLoadImage_triggered(){
    filePath = QFileDialog::getOpenFileName(0, "Load Image", filePath, "*.jpg *.png *.jpeg");
    if(filePath == "")
        return;

    currentImage.load(filePath);
    isImageNow = true;
    fillBlocks();
}

void MainWindow::on_actionOrder_triggered(){
    for(int i = 0; i < blockPosition.length(); i++)
        blockPosition[i] = i;

    for(int i = 0; i < blockPosition.length(); i++)
        if(blockPosition[i] == blockPosition.length() - 1){
            poorBlockNum = i;
            break;
        }

    ui->centralWidget->layout()->setSpacing(5);

    isWin = false;
    //order the blocks
    fillBlocks();

    myTimer.restart();
}

void MainWindow::on_actionStandartBlocks_triggered(){
    isImageNow = false;
    fillBlocks();
}

void MainWindow::on_actionLoadBackground_triggered(){
    filePath = QFileDialog::getOpenFileName(0, "Load Image", filePath, "*.jpg *.png *.jpeg");
    if(filePath == "")
        return;

    currentBackground.load(filePath);
    isBackGroundNow = true;

    QBrush *brush = new QBrush();
    brush->setTextureImage(currentBackground.scaled(this->centralWidget()->width(), this->centralWidget()->height(),
                                                    Qt::IgnoreAspectRatio,Qt::FastTransformation));
    QPalette palet(QPalette::Background);
    palet.setBrush(QPalette::Background, *brush);
    this->centralWidget()->setPalette(palet);
    this->centralWidget()->setAutoFillBackground(true);
    this->centralWidget()->palette();
}

void MainWindow::on_actionStandartBackground_triggered(){
    isBackGroundNow = false;
    const QPalette palet(QPalette::Background, QColor(150,100,250,255));
    this->centralWidget()->setPalette(palet);
}

void MainWindow::on_actionAbout_triggered(){
    aboutWindow->show();
}

void MainWindow::fillBlocks(){
    if(isImageNow){
        int x, y, w, h;

        w = (currentImage.width() / fifteenWidth);
        h = (currentImage.height() / fifteenHeight);

        poorBlockImage = QImage(w,h,QImage::Format_ARGB32);
        poorBlockImage.fill(qRgba(255,255,255,0));

        for(int i = 0; i < blockPosition.length(); i++){
            if(i != poorBlockNum){
                x = (blockPosition[i] % fifteenWidth) * w;
                y = (blockPosition[i] / fifteenWidth) * h;
                QImage tempImage = currentImage.copy(x,y,w,h);
                labels[i]->setPixmap(QPixmap::fromImage(tempImage).scaled(100,100,Qt::IgnoreAspectRatio,Qt::FastTransformation));
            }
            else
                labels[i]->setPixmap(QPixmap::fromImage(poorBlockImage).scaled(100,100,Qt::IgnoreAspectRatio,Qt::FastTransformation));
        }
    }
    else{
        for(int i = 0; i < blockPosition.length(); i++)
            if(i != poorBlockNum)
                labels[i]->setText(QString("%1").arg(blockPosition[i] + 1));
            else
                labels[i]->setText(QString("*"));
    }

    //work with background
    if(isWin){
        for(int i = 0; i < labels.length(); i++){
            const QPalette palet(QPalette::Background, QColor(100,50,200,0));
            labels[i]->setPalette(palet);
            labels[i]->setAutoFillBackground(true);
            labels[i]->setText(QString(""));
        }
    }
    else
        for(int i = 0; i < labels.length(); i++){
            if(i != poorBlockNum){
                const QPalette palet(QPalette::Background, QColor(100,50,200,255));
                labels[i]->setPalette(palet);
            }
            else{
                const QPalette palet(QPalette::Background, QColor(100,50,200,0));
                labels[i]->setPalette(palet);
            }
        }
}

bool MainWindow::blockSwap(int index1, int index2){
    int xIndex1 = index1 % fifteenWidth;
    int yIndex1 = index1 / fifteenWidth;
    int xIndex2 = index2 % fifteenWidth;
    int yIndex2 = index2 / fifteenWidth;
    if(index1 > -1 && index1 < blockPosition.length() &&
       index2 > -1 && index2 < blockPosition.length() &&
       (xIndex1 == xIndex2 || yIndex1 == yIndex2)){
        //int w = (currentImage.width() / fifteenWidth);
        //int h = (currentImage.height() / fifteenHeight);

        if(isImageNow){
            QImage tempImage = labels[index1]->pixmap()->toImage();
            labels[index2]->setPixmap(QPixmap::fromImage(tempImage).scaled(100, 100, Qt::IgnoreAspectRatio, Qt::FastTransformation));
            labels[index1]->setPixmap(QPixmap::fromImage(poorBlockImage).scaled(100, 100, Qt::IgnoreAspectRatio, Qt::FastTransformation));
        }
        else{
            QString tempString = labels[index2]->text();
            labels[index2]->setText(labels[index1]->text());
            labels[index1]->setText(tempString);
        }

        int temp = blockPosition[index1];
        blockPosition[index1] = blockPosition[index2];
        blockPosition[index2] = temp;

        //background
        QColor tempColor = labels[index1]->palette().color(QPalette::Background);
        const QPalette palet1(QPalette::Background, labels[index2]->palette().color(QPalette::Background));
        labels[index1]->setPalette(palet1);

        const QPalette palet2(QPalette::Background, tempColor);
        labels[index2]->setPalette(palet2);

        poorBlockNum = index1;

        checkWin();

        return true;
    }
    else
        return false;
}

void MainWindow::checkWin(){
    bool winCheck = true;
    for(int i = 0 ; i < blockPosition.length(); i++){
        if(blockPosition[i] != i){
            winCheck = false;
            break;
        }
    }

    if(winCheck){
        QMessageBox::about(this, "Congratulations", "You are WINNER!)");
        poorBlockNum = -1;
        ui->centralWidget->layout()->setSpacing(0);
        isWin = true;
        fillBlocks();

        if(dialogWindow != NULL)
            delete dialogWindow;
        dialogWindow = new Dialog(this, (myTimer.currentTime().hour() - myTimer.hour()) * 24 +
                                  (myTimer.currentTime().minute() - myTimer.minute()) * 60 +
                                  (myTimer.currentTime().second() - myTimer.second()));
        dialogWindow->show();

    }
    else{
        ui->centralWidget->layout()->setSpacing(5);
        isWin = false;
    }
}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress )
    {
        for(int i = 0 ; i < labels.length(); i++){
            if(target == labels[i])
            {
                int xPos = i % fifteenWidth;
                int yPos = i / fifteenWidth;

                if(((xPos + 1) + fifteenWidth * yPos) == poorBlockNum ||
                   ((xPos - 1) + fifteenWidth * yPos) == poorBlockNum ||
                   (xPos + fifteenWidth * (yPos + 1)) == poorBlockNum ||
                   (xPos + fifteenWidth * (yPos - 1)) == poorBlockNum)
                {
                    blockSwap(i, poorBlockNum);
                }
                break;
            }
        }
    }
    else if(event->type() == QEvent::Resize && isBackGroundNow){
        QBrush brush;
        brush.setTextureImage(currentBackground.scaled(this->centralWidget()->width(), this->centralWidget()->height(),
                                                       Qt::IgnoreAspectRatio,Qt::FastTransformation));
        QPalette palet(QPalette::Background);
        palet.setBrush(QPalette::Background, brush);
        this->centralWidget()->setPalette(palet);
    }
    return QMainWindow::eventFilter(target, event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int xPoorBlock = poorBlockNum % fifteenWidth;
    int yPoorBlock = poorBlockNum / fifteenWidth;

    int xBlock = -1;
    int yBlock = -1;

    switch (event->key()) {
    case Qt::Key_Up:
        xBlock = xPoorBlock;
        yBlock = yPoorBlock + 1;
        break;
    case Qt::Key_Down:
        xBlock = xPoorBlock;
        yBlock = yPoorBlock - 1;
        break;
    case Qt::Key_Left:
        xBlock = xPoorBlock + 1;
        yBlock = yPoorBlock;
        break;
    case Qt::Key_Right:
        xBlock = xPoorBlock - 1;
        yBlock = yPoorBlock;
        break;
    default:
        break;
    }

    int blockPos = (xBlock + fifteenWidth * yBlock);
    blockSwap(blockPos, poorBlockNum);
}
