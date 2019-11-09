module com.ircsk2.irc.client {
    requires javafx.controls;
    requires javafx.fxml;

    opens com.ircsk2.irc.client to javafx.fxml;
    exports com.ircsk2.irc.client;
}