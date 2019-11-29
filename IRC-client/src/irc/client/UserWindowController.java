/**
 * Sample Skeleton for 'UserWindow.fxml' Controller Class
 */
package irc.client;

import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.ColorPicker;
import javafx.scene.control.TextField;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

public class UserWindowController {

    private FXMLDocumentController parentController;

    private String username;
    private String color = "#ffffff";

    void setParentController(FXMLDocumentController controller) {
        this.parentController = controller;
        this.setUsername(parentController.getUsername());
    }

    @FXML // ResourceBundle that was given to the FXMLLoader
    private ResourceBundle resources;

    @FXML // URL location of the FXML file that was given to the FXMLLoader
    private URL location;

    @FXML // fx:id="okUserButton"
    private Button okUserButton; // Value injected by FXMLLoader

    @FXML // fx:id="anulujUserButton"
    private Button anulujUserButton; // Value injected by FXMLLoader

    @FXML // fx:id="colorPalette"
    private ColorPicker colorPalette; // Value injected by FXMLLoader

    @FXML // fx:id="usernameField"
    private TextField usernameField; // Value injected by FXMLLoaderF

    @FXML
    void anulujUserButtonAction(ActionEvent event) {
        Stage stage = (Stage) this.anulujUserButton.getScene().getWindow();
        this.parentController.setUserWindow(false);
        stage.close();
    }
    
    /**
     * @return the username
     */
    public String getUsername() {
        return username;
    }

    /**
     * @param username the username to set
     */
    public void setUsername(String username) {
        this.username = username;
    }

    /**
     * @return the usernameField
     */
    public TextField getUsernameField() {
        return usernameField;
    }

    @FXML
    void okUserButtonAction(ActionEvent event) {
        
        this.setUsername(this.getUsernameField().getText());
        this.parentController.setUsername(getUsername());
        
        this.parentController.getConnection().sendMessage("$username " + this.getUsernameField().getText());
        this.parentController.getConnection().sendMessage("$color " + this.color);
        Stage stage = (Stage) this.okUserButton.getScene().getWindow();
        this.parentController.setUserWindow(false);
        stage.close();
    }

    private String format(double val) {
        String in = Integer.toHexString((int) Math.round(val * 255));
        return in.length() == 1 ? "0" + in : in;
    }

    private String toHexString(Color value) {
        return "#" + (format(value.getRed()) + format(value.getGreen()) + format(value.getBlue()))
                .toUpperCase();
    }

    @FXML
    void colorAction(ActionEvent event) {
        this.color = toHexString(this.colorPalette.getValue());
        this.getUsernameField().setStyle("-fx-text-fill: " + this.color + "; -fx-background-color: #000000; -fx-border-color: #ffffff;");
    }

    @FXML // This method is called by the FXMLLoader when initialization is complete
    void initialize() {
        assert okUserButton != null : "fx:id=\"okUserButton\" was not injected: check your FXML file 'UserWindow.fxml'.";
        assert anulujUserButton != null : "fx:id=\"anulujUserButton\" was not injected: check your FXML file 'UserWindow.fxml'.";
        this.getUsernameField().setStyle("-fx-text-fill: #ffffff; -fx-background-color: #000000; -fx-border-color: #ffffff;");
    }
}
