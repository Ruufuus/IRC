/**
 * Sample Skeleton for 'FXMLDocument.fxml' Controller Class
 */
package irc.client;

import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.ListCell;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.scene.control.MenuItem;
import javafx.stage.Stage;
import javafx.util.Callback;

public class FXMLDocumentController {

    private Connection connection = null;
    private boolean polaczWindow = false;
    private boolean kanalWindow = false;

    /**
     * @return the connection
     */
    public Connection getConnection() {
        return connection;
    }

    /**
     * @param connection the connection to set
     */
    public void setConnection(Connection connection) {
        this.connection = connection;
    }

    /**
     * @return the polaczWindow
     */
    public boolean isPolaczWindow() {
        return polaczWindow;
    }

    /**
     * @param polaczWindow the polaczWindow to set
     */
    public void setPolaczWindow(boolean polaczWindow) {
        this.polaczWindow = polaczWindow;
    }

    /**
     * @return the kanalWindow
     */
    public boolean isKanalWindow() {
        return kanalWindow;
    }

    /**
     * @param kanalWindow the kanalWindow to set
     */
    public void setKanalWindow(boolean kanalWindow) {
        this.kanalWindow = kanalWindow;
    }

    @FXML
    private TextField messField;

    @FXML
    private MenuItem polaczSubMenu;

    @FXML
    private MenuItem dolaczSubMenu;

    @FXML
    private MenuItem utworzSubMenu;

    @FXML
    private ListView<?> canalList;

    @FXML
    private ListView<?> userList;

    @FXML
    private ListView<String> messageList;

    void appendMessages(String message) {
        this.messageList.getItems().add(message);
        //System.out.println(this.messageList.getItems());
    }

    @FXML
    void kanalAction(ActionEvent event) {
        try {
            if (!isKanalWindow()) {
                FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("CanalWindow.fxml"));
                Parent root = fxmlLoader.load();

                CanalWindowController canalController = fxmlLoader.getController();
                canalController.setParentController(this);

                Stage stage = new Stage();
                stage.setTitle("Zarządzanie Kanałami");
                stage.setScene(new Scene(root));
                this.setKanalWindow(true);
                stage.show();
            }
        } catch (IOException ex) {
            Logger.getLogger(FXMLDocumentController.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @FXML
    void polaczAction(ActionEvent event) {
        try {
            if (!isPolaczWindow()) {
                FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("ConnectionWindow.fxml"));
                Parent root = fxmlLoader.load();

                ConnectionWindowController connectionController = fxmlLoader.getController();

                connectionController.setParentController(this);

                Stage stage = new Stage();
                stage.setTitle("Połącz");
                stage.setScene(new Scene(root));
                this.setPolaczWindow(true);
                stage.show();
            }
        } catch (IOException ex) {
            Logger.getLogger(FXMLDocumentController.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @FXML
    void sendMessage(ActionEvent event) throws IOException {
        if (this.getConnection() != null) {
            String message = this.messField.getText();
            this.getConnection().sendMessage(message);
            messField.clear();
        }

    }

    @FXML
    void initialize() {
        assert messField != null : "fx:id=\"messField\" was not injected: check your FXML file 'FXMLDocument.fxml'.";
        assert polaczSubMenu != null : "fx:id=\"polaczSubMenu\" was not injected: check your FXML file 'FXMLDocument.fxml'.";
        assert dolaczSubMenu != null : "fx:id=\"dolaczSubMenu\" was not injected: check your FXML file 'FXMLDocument.fxml'.";
        assert utworzSubMenu != null : "fx:id=\"utworzSubMenu\" was not injected: check your FXML file 'FXMLDocument.fxml'.";
        assert canalList != null : "fx:id=\"canalList\" was not injected: check your FXML file 'FXMLDocument.fxml'.";
        assert userList != null : "fx:id=\"userList\" was not injected: check your FXML file 'FXMLDocument.fxml'.";
        assert messageList != null : "fx:id=\"messageList\" was not injected: check your FXML file 'FXMLDocument.fxml'.";
        messageList.setCellFactory(new Callback<ListView<String>, ListCell<String>>() {
            @Override
            public ListCell<String> call(ListView<String> stringListView) {
                return new ListCell<String>() {
                    @Override
                    protected void updateItem(String s, boolean empty) {
                        super.updateItem(s, empty);    //To change body of overridden methods use File | Settings | File Templates.
                        if (!empty) {
                            String color = s.split(" ")[1].split("@")[1];
                                setStyle("-fx-text-fill: " + color);
                                setText(s);
                                
                        }
                    }
                };
            }
        });
    }
}
