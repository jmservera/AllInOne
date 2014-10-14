using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace AIOEngine
{
    public enum LogLevel
    {
        Information,
        Warning,
        Error
    }

    /// <summary>
    /// A log item
    /// </summary>
    public class LogItem
    {
        /// <summary>
        /// A tag used for organize the messages
        /// </summary>
        public StringBuilder Tag { get; set; }

        /// <summary>
        /// The message
        /// </summary>
        public StringBuilder Text { get; set; }

        /// <summary>
        /// The level asigned to this item
        /// </summary>
        public LogLevel Level { get; set; }

        /// <summary>
        /// Default constructor
        /// </summary>
        /// <param name="tag"></param>
        /// <param name="text"></param>
        /// <param name="level"></param>
        public LogItem(StringBuilder tag, StringBuilder text, LogLevel level)
        {
            Tag = tag;
            Text = text;
            Level = level;
        }
    }

    /// <summary>
    /// A log class that can comunicate with WPF
    /// </summary>
    public class Log
    {
        /// <summary>
        /// List of <see cref="LogItem"/>. YO MUST NOT ADD Items manually to this list, you MUST use the Add... methods
        /// </summary>
        public ObservableCollection<LogItem> Items { get; private set; }

        /// <summary>
        /// Used for stream output
        /// </summary>
        StreamWriter writer;

        /// <summary>
        /// Default constructor
        /// </summary>
        public Log()
        {
            Items = new ObservableCollection<LogItem>();

        }

        /// <summary>
        /// Frees the resources
        /// </summary>
        public void Destroy()
        {
            if(writer!=null)
            {
                writer.Close();

                writer = null;
            }
        }

        /// <summary>
        /// Indicates that the log must write to a file too
        /// </summary>
        /// <param name="path">FullPath</param>
        public void SetFileOutput(string path)
        {
            // Creates a new file with UTF8 format

            writer = new StreamWriter(path,false, Encoding.UTF8);

            writer.WriteLine("Created at " + DateTime.Now.ToString());

            writer.Flush();
        }

        /// <summary>
        /// Adds an information message to the log
        /// </summary>
        /// <param name="tag"></param>
        /// <param name="message"></param>
        public void AddInformation(StringBuilder tag, StringBuilder message)
        {
            AddItem(tag, message, LogLevel.Information);
        }

        /// <summary>
        /// Adds a warning message to the log
        /// </summary>
        /// <param name="tag"></param>
        /// <param name="message"></param>
        public void AddWarning(StringBuilder tag, StringBuilder message)
        {
            AddItem(tag, message, LogLevel.Warning);
        }

        /// <summary>
        /// Adds an error message to the log
        /// </summary>
        /// <param name="tag"></param>
        /// <param name="message"></param>
        public void AddError(StringBuilder tag, StringBuilder message)
        {
            AddItem(tag, message, LogLevel.Error);
        }

        public void AddItem(StringBuilder tag, StringBuilder message, LogLevel level)
        {
            Items.Add(new LogItem(tag, message, level));

            if (writer != null)
            {
                writer.WriteLine(DateTime.Now.ToString() + "\t\t" + level.ToString() + "\t\t" + tag + "\t\t\t\t" + message);
                writer.Flush();
            }
        }
    }
}
