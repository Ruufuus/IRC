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
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.scene.text.TextFlow;
import javafx.scene.control.MenuItem;
import javafx.stage.Stage;

public class FXMLDocumentController {

    static Connection connection = null;
    static boolean polaczWindow = false;
    static boolean kanalWindow = false;

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
    
    void appendMessages(String message){
        this.messageList.getItems().add(message);
        System.out.println(this.messageList.getItems());
    }

    @FXML
    void kanalAction(ActionEvent event) {
        try {
            if (!kanalWindow) {
                FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("CanalWindow.fxml"));
                Parent root = fxmlLoader.load();
                Stage stage = new Stage();
                stage.setTitle("Zarządzanie Kanałami");
                stage.setScene(new Scene(root));
                FXMLDocumentController.kanalWindow = true;
                stage.show();
            }
        } catch (IOException ex) {
            Logger.getLogger(FXMLDocumentController.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @FXML
    void polaczAction(ActionEvent event) {
        try {
            if (!polaczWindow) {
                FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("ConnectionWindow.fxml"));
                Parent root = fxmlLoader.load();
                
                ConnectionWindowController connectionController = fxmlLoader.getController();
                
                connectionController.setParentController(this);
                
                Stage stage = new Stage();
                stage.setTitle("Połącz");
                stage.setScene(new Scene(root));
                FXMLDocumentController.polaczWindow = true;
                stage.show();
            }
        } catch (IOException ex) {
            Logger.getLogger(FXMLDocumentController.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @FXML
    void sendMessage(ActionEvent event) throws IOException {
        if (FXMLDocumentController.connection != null) {
            String message = this.messField.getText();
            FXMLDocumentController.connection.sendMessage(message);
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
        //assert messageList != null : "fx:id=\"messageList\" was not injected: check your FXML file 'FXMLDocument.fxml'.";

    }
}
