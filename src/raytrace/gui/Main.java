package raytrace.gui;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class Main extends Application {


    @Override
    public void start(Stage primaryStage) throws Exception{

        FXMLLoader loader = new FXMLLoader(getClass().getResource("mainWindow.fxml"));
        Controller controller = new Controller();
        loader.setController(controller);

        Parent root = loader.load();

        controller.init();

        primaryStage.setTitle("Hello World");
        primaryStage.setScene(new Scene(root, 1280, 800));
        primaryStage.show();
    }


    public static void main(String[] args) {
        launch(args);
    }
}
