using System;
using System.Collections.Generic;
using System.ComponentModel;

namespace ConfigApp
{
    public class BitsMenuItem : INotifyPropertyChanged
    {
        public string Text { get; private set; }
        public BitsMenuItem Parent;
        public List<BitsMenuItem> Children { get; private set; }
        public event PropertyChangedEventHandler PropertyChanged;
        private int m_Bits;
        public int Bits
        {
            get
            {
                return m_Bits < 0 ? -1 : m_Bits;
            }
            set
            {
                m_Bits = value;

                NotifyFieldsUpdated();
            }
        }
        public string IsConfigVisible
        {
            get
            {
                return Children.Count == 0 ? "Visible" : "Hidden";
            }
        }
        public bool IsEventEnabled
        {
            get
            {
                return Bits >= 0;
            }
        }

        public string IsBranch
        {
            get
            {
                return Bits < 0 ? "Collapsed" : "Visible";
            }
        }

        public BitsMenuItem(string text, BitsMenuItem parent = null)
        {
            Text = text;
            Parent = parent;
            Children = new List<BitsMenuItem>();
            m_Bits = parent == null ? -1 : 0;
        }

        public void AddChild(BitsMenuItem menuItem)
        {
            if (menuItem != null)
            {
                menuItem.Parent = this;
                if (menuItem.Bits < 0)
                {
                    menuItem.Bits = 0;
                }
                Children.Add(menuItem);
            }
        }

        private void NotifyFieldsUpdated()
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Bits"));
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("IsConfigVisible"));
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("IsConfigEnabled"));
        }
    }
}
