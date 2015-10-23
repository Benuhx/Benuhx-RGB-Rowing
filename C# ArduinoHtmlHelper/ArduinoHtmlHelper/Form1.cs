using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
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
            rtbResult.Text = "";
            var i = 0;
            foreach(String curLine in rtbHtml.Text.Split('\n'))
            {
                if(string.IsNullOrWhiteSpace(curLine)) continue;
                var bLine = curLine.Replace(@"""", @"\""");
                var start = bLine.IndexOf("#!", StringComparison.Ordinal);
                var ende = bLine.IndexOf("!#", StringComparison.Ordinal);
                var useF = true;
                if (start > 0 && ende > 0)
                {
                    useF = false;
                    var txt = bLine.Substring(start + 2, ende - start - 2);
                    bLine = bLine.Remove(start, ende - start + 2);
                    bLine = bLine.Insert(start, string.Format(@""" + {0} + """, txt));
                }
                bLine = bLine.Trim();
                if (i == 0)
                {
                    rtbResult.Text = string.Format(useF ? @"String html = F(""{0}"");" : @"String html = ""{0}"";", bLine);
                    rtbResult.Text += Environment.NewLine + "html += newLine;" + Environment.NewLine;
                } else
                {
                    rtbResult.Text += string.Format(useF ? @"html += F(""{0}"");" : @"html += ""{0}"";", bLine);
                    rtbResult.Text += Environment.NewLine + "html += newLine;" + Environment.NewLine;
                }
                i++;                
            }
            Clipboard.SetText(rtbResult.Text);
        }
    }
}
