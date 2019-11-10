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

    @FXML // ResourceBundle that was given to the FXMLLoader
    private ResourceBundle resources;

    @FXML // URL location of the FXML file that was given to the FXMLLoader
    private URL location;

    @FXML // fx:id="messField"
    private TextField messField; // Value injected by FXMLLoader

    @FXML // fx:id="textFlow"
    private TextFlow textFlow; // Value injected by FXMLLoader

    @FXML // fx:id="polaczSubMenu"
    private MenuItem polaczSubMenu; // Value injected by FXMLLoader

    @FXML // fx:id="dolaczSubMenu"
    private MenuItem dolaczSubMenu; // Value injected by FXMLLoader

    @FXML // fx:id="utworzSubMenu"
    private MenuItem utworzSubMenu; // Value injected by FXMLLoader

    @FXML // fx:id="canalList"
    private ListView<?> canalList; // Value injected by FXMLLoader

    @FXML // fx:id="userList"
    private ListView<?> userList; // Value injected by FXMLLoader

    @FXML
    void kanalAction(ActionEvent event) {

    }

    @FXML
    void polaczAction(ActionEvent event) {
        try {
            FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("ConnectionWindow.fxml"));
            Parent root = fxmlLoader.load();
            Stage stage = new Stage();
            stage.setTitle("Połącz");
            stage.setScene(new Scene(root));
            stage.show();
        } catch (IOException ex) {
            Logger.getLogger(FXMLDocumentController.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @FXML
    void sendMessage(ActionEvent event) throws IOException {
        String message = this.messField.getText();
        this.connection.sendMessage(message);
        messField.clear();

    }

    @FXML // This method is called by the FXMLLoader when initialization is complete
    void initialize() {
        assert messField != null : "fx:id=\"messField\" was not injected: check your FXML file 'FXMLDocument.fxml'.";
        assert textFlow != null : "fx:id=\"textFlow\" was not injected: check your FXML file 'FXMLDocument.fxml'.";
        assert polaczSubMenu != null : "fx:id=\"polaczSubMenu\" was not injected: check your FXML file 'FXMLDocument.fxml'.";
        assert dolaczSubMenu != null : "fx:id=\"dolaczSubMenu\" was not injected: check your FXML file 'FXMLDocument.fxml'.";
        assert utworzSubMenu != null : "fx:id=\"utworzSubMenu\" was not injected: check your FXML file 'FXMLDocument.fxml'.";
        assert canalList != null : "fx:id=\"canalList\" was not injected: check your FXML file 'FXMLDocument.fxml'.";
        assert userList != null : "fx:id=\"userList\" was not injected: check your FXML file 'FXMLDocument.fxml'.";

    }
}
