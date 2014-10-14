using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using GraphicsEngineSpace;
using AIOEngine.Graphics;

namespace AllInOne
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private VisualEngine editorEngine;

        TestRenderControl renderControl;
        SpriteRenderControl spriteRenderControl;
        StringBuilder tag=new StringBuilder("MainWindow");

        public MainWindow()
        {
            InitializeComponent();
            Loaded += Window1_Loaded;
        }

        void Window1_Loaded(object sender, RoutedEventArgs e)
        {
            /*
             * VERY IMPORTANT NOTE
             * THIS CODE HAS NO SENSE HERE, BUT IF IT'S NOT EXECUTED, THE ENGINE GIVES A ENGINERUNTIMEEXCEPTION WHEN USED IN OTHER PLACES
            */
            
            RenderableMeshPart meshPart = new RenderableMeshPart(6, 6, 0);

            /*
             * END OF THE STUPID CODE :(
            */

            // Create the engined that will be used for the editor
            editorEngine = new VisualEngine(new StringBuilder("Editor"));

            // Add a relative path where the textures are located
            editorEngine.Textures.AddRelativePath("Textures");
            
            spriteRenderControl = new SpriteRenderControl(editorEngine, new StringBuilder("SpriteControl"));

            spriteRenderControl.Margin = new Thickness(10, 50, 10, 10);

            Grid.SetColumn(spriteRenderControl, 0);

            ctGrid.Children.Add(spriteRenderControl);

            renderControl = new TestRenderControl(editorEngine, new StringBuilder("Control"));

            Grid.SetColumn(renderControl, 1);
            
            renderControl.Margin = new Thickness(10, 50, 10, 10);

            ctGrid.Children.Add(renderControl);

            
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            editorEngine.Log.AddInformation(tag, new StringBuilder("control closed"));

            editorEngine.Destroy();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            ctGrid.Children.Remove(renderControl);
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {

        }
    }
}
