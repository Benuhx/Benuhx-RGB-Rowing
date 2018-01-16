using NUglify;
using System;
using System.Windows.Forms;

namespace ArduinoHtmlHelper
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            var minifiziert = Uglify.Html(rtbHtml.Text);
            if(minifiziert.HasErrors)
            {
                MessageBox.Show("Fehler: " + string.Join(Environment.NewLine, minifiziert.Errors));
                return;
            }
            rtbHtml.Text = minifiziert.Code;
            for (var i = 0; i < rtbHtml.Text.Split('\n').Length; i++)
            {
                var curLine = rtbHtml.Text.Split('\n')[i];
                if (string.IsNullOrWhiteSpace(curLine)) continue;
                var bLine = curLine.Replace(@"""", @"\"""); // " escapen
                var lineBeendet = false;
                var useFFunction = true; //Ist die F("") um Speicher zu sparen
                while (!lineBeendet) //Sonst wird immer nur das erste Vorkommen ersetzt
                {
                    var start = bLine.IndexOf("#!", StringComparison.Ordinal);
                    var ende = bLine.IndexOf("!#", StringComparison.Ordinal);
                    if (start > 0 && ende > 0)
                    {
                        useFFunction = false;
                        var txt = bLine.Substring(start + 2, ende - start - 2);
                        bLine = bLine.Remove(start, ende - start + 2);
                        bLine = bLine.Insert(start, string.Format(@""" + {0} + """, txt));
                    }
                    else
                    {
                        lineBeendet = true;
                    }
                }

                bLine = bLine.Trim();
                if (i == 0)
                {
                    rtbResult.Text =
                        string.Format(useFFunction ? @"String html = F(""{0}"");" : @"String html = ""{0}"";", bLine);
                    rtbResult.Text += Environment.NewLine;
                }
                else
                {
                    rtbResult.Text += string.Format(useFFunction ? @"html += F(""{0}"");" : @"html += ""{0}"";", bLine);
                    rtbResult.Text += Environment.NewLine;
                }
            }
            Clipboard.SetText(rtbResult.Text);
        }
    }
}
