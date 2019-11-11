/**
 * Sample Skeleton for 'ConnectionWindow.fxml' Controller Class
 */

package irc.client;

import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

public class ConnectionWindowController {
    
    private FXMLDocumentController parentController ;
    
    void setParentController(FXMLDocumentController controller){
        this.parentController = controller;
    }
    
    @FXML // ResourceBundle that was given to the FXMLLoader
    private ResourceBundle resources;

    @FXML // URL location of the FXML file that was given to the FXMLLoader
    private URL location;

    @FXML // fx:id="adresTextField"
    private TextField adresTextField; // Value injected by FXMLLoader

    @FXML // fx:id="portTextField"
    private TextField portTextField; // Value injected by FXMLLoader

    @FXML // fx:id="okConnectionButton"
    private Button okConnectionButton; // Value injected by FXMLLoader

    @FXML // fx:id="anulujConnectionButton"
    private Button anulujConnectionButton; // Value injected by FXMLLoader

    @FXML
    void anulujConnectionButtonAction(ActionEvent event) {
        Stage stage = (Stage) this.anulujConnectionButton.getScene().getWindow();
        FXMLDocumentController.polaczWindow = false;
        stage.close();
    }

    @FXML
    void okConnectionButtonAction(ActionEvent event) {
        FXMLDocumentController.connection = new Connection(this.adresTextField.getText(), Integer.parseInt(this.portTextField.getText()), parentController);
        new Thread(FXMLDocumentController.connection).start();
        Stage stage = (Stage) this.okConnectionButton.getScene().getWindow();
        FXMLDocumentController.polaczWindow = false;
        stage.close();

    }

    @FXML // This method is called by the FXMLLoader when initialization is complete
    void initialize() {
        assert adresTextField != null : "fx:id=\"adresTextField\" was not injected: check your FXML file 'ConnectionWindow.fxml'.";
        assert portTextField != null : "fx:id=\"portTextField\" was not injected: check your FXML file 'ConnectionWindow.fxml'.";
        assert okConnectionButton != null : "fx:id=\"okConnectionButton\" was not injected: check your FXML file 'ConnectionWindow.fxml'.";
        assert anulujConnectionButton != null : "fx:id=\"anulujConnectionButton\" was not injected: check your FXML file 'ConnectionWindow.fxml'.";

    }
}
