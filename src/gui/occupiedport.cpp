#include "occupiedport.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

occupiedport::occupiedport(QWidget *parent)
    : QDialog(parent)
{
    // Set window title and size.
    setWindowTitle("Occupied Port");
    setFixedSize(300, 150);

    // Create the informational label.
    infoLabel = new QLabel("The current port is occupied. Please enter a new one:", this);
    infoLabel->setWordWrap(true);

    // Create the input field for the new port.
    portInput = new QLineEdit(this);
    portInput->setPlaceholderText("Enter new port");

    // Create the Confirm button.
    confirmButton = new QPushButton("Confirm", this);
    confirmButton->setFixedWidth(100);

    // Connect the Confirm button to its event handler.
    connect(confirmButton, &QPushButton::clicked, this, &occupiedport::onConfirmButtonClicked);

    // Create the main layout.
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(portInput);

    // Create a horizontal layout to center the button.
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // Display an initial popup message.
    QMessageBox::information(this, "Occupied Port", "The current port is occupied. Please enter a new port.");
}

occupiedport::~occupiedport()
{
    // No need to delete the child widgets, they are deleted automatically.
}

void occupiedport::onConfirmButtonClicked()
{
    // Retrieve the text from the port input.
    QString portText = portInput->text().trimmed();

    // If the port field is empty, show a warning popup.
    if (portText.isEmpty()) {
        QMessageBox::warning(this, "Error", "The port entered is not valid. Please enter a correct port.");
        return;
    }
    accept();
}
