using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Excel = Microsoft.Office.Interop.Excel;
using System.Reflection;
//using Microsoft.Office.Core;//使用Nothing
using System.Runtime.InteropServices;//导入dll
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;

namespace WindowsFormsApp1
{
    internal class Excel_deal
    {
        public Excel.Application myApp = null;

        public Excel.Workbook workbook = null;

        public Excel.Worksheet worksheet = null;

        public void Creat_Excel_APP()
        {
            myApp = new Excel.Application();
            myApp.DisplayAlerts = false;

            workbook = myApp.Workbooks.Add(true);
        }

        public void Change_Excel_Worksheets(string name)
        {
            for (byte i = 1; i < 10; i++)
            {
                if (workbook.Sheets[i].name == name)
                {
                    worksheet = (Excel.Worksheet)workbook.Sheets[i];
                    break;
                }
            }

            worksheet.Activate();
        }

        public void Add_Excel_Worksheets(string name)
        {
            if (null == workbook)
            {
                return;
            }
            worksheet = workbook.Worksheets.Add();
            worksheet.Name = name;
        }

        public void Write_Excle_Worksheets(UInt32 l, UInt32 h, string data)
        {
            if (null == worksheet)
            {
                return;
            }
            worksheet.Cells[h, l] = data;
        }

        public void set_cells_background_red(UInt32 l, UInt32 h)
        {
            worksheet.Cells[h, l].Interior.Color = Excel.XlRgbColor.rgbRed;
        }

        public void set_cells_background_white(UInt32 l, UInt32 h)
        {
            worksheet.Cells[h, l].Interior.Color = Excel.XlRgbColor.rgbWhite;
        }

        public void SaveAs_Excel_WorkBook_only(string path)
        {
            if (null == workbook)
            {
                return;
            }

            workbook.SaveAs(path);
        }

        public void SaveAs_Excel_WorkBook(string path)
        {
            if (null == workbook)
            {
                return;
            }              

            workbook.SaveAs(path);

            workbook.Close();

            workbook = null;
        }

        public void Close_Excel_WorkBook()
        {
            workbook.Close();

            workbook = null;
        }


        [DllImport("kernel32.dll")]
        public static extern IntPtr _lopen(string lpPathName, int iReadWrite);

        [DllImport("kernel32.dll")]
        public static extern bool CloseHandle(IntPtr hObject);
        public const int OF_READWRITE = 2;
        public const int OF_SHARE_DENY_NONE = 0x40;
        public static readonly IntPtr HFILE_ERROR = new IntPtr(-1);

        public bool IsFileOpen(string path)
        {
            if (!File.Exists(path))
            {
                return false;
            }
            IntPtr vHandle = _lopen(path, OF_READWRITE | OF_SHARE_DENY_NONE);//windows Api上面有定义扩展方法
            if (vHandle == HFILE_ERROR)
            {
                return true;
            }
            CloseHandle(vHandle);
            return false;
        }
    }
}
