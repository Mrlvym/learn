using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsFormsApp1
{
    //创建环形缓冲区类
    public class RingBuff
    {
        public UInt16 head;
        public UInt16 tail;
        public UInt16 capacity;
        public byte[] buf;

        public RingBuff()
        {
            head = 0;
            tail = 0;
            //capacity = 10240 * 10;
            capacity = 65535;

            buf = new byte[capacity];
        }

        public void clear_ringbuff()
        {
            head = 0;
            tail = 0;
            //capacity = 10240 * 10;
            capacity = 65535;
        }

        public UInt16 rb_can_read()
        {
            if (head <= tail)
                return (UInt16)(tail - head);
            else
                return (UInt16)(capacity - head + tail);
        }

        public UInt16 rb_can_write()
        {
            return (UInt16)(capacity - this.rb_can_read());
        }

        public UInt16 rb_read_buf(byte[] buf, byte[] data, UInt16 count)
        {
            if (head < tail)
            {
                UInt16 c_count;
                UInt16 ret = this.rb_can_read();
                if (ret < count)
                    c_count = ret;
                else
                    c_count = count;

                Buffer.BlockCopy(buf, this.head, data, 0, c_count);

                this.head = (UInt16)((this.head + c_count) % capacity);

                return c_count;
            }
            else
            {
                if (count < (capacity - this.head))
                {
                    Buffer.BlockCopy(buf, this.head, data, 0, count);
                    this.head = (UInt16)((this.head + count) % capacity);

                    return count;
                }
                else
                {
                    Buffer.BlockCopy(buf, this.head, data, 0, capacity - this.head);
                    Buffer.BlockCopy(buf, 0, data, capacity - this.head, count - (capacity - this.head));

                    this.head = (UInt16)((this.head + count) % capacity);
                    return count;
                }
            }
            //return 0;
        }

        public UInt16 rb_write_buf(byte[] buf, byte[] data, UInt16 count)
        {
            if (count >= rb_can_write())
                return 0;
            if (this.head <= this.tail)
            {
                UInt16 sy_count = (UInt16)(capacity - this.tail);
                if (count < sy_count)
                {
                    Buffer.BlockCopy(data, 0, buf, this.tail, count);

                    this.tail = (UInt16)((this.tail + count) % capacity);
                    return count;
                }
                else
                {
                    Buffer.BlockCopy(data, 0, buf, this.tail, sy_count);
                    Buffer.BlockCopy(data, sy_count, buf, 0, count - sy_count);

                    this.tail = (UInt16)((this.tail + count) % capacity);
                    return count;
                }
            }
            else
            {
                Buffer.BlockCopy(data, 0, buf, this.tail, count);

                this.tail = (UInt16)((this.tail + count) % capacity);
                return count;
            }

            //return 0;
        }
    };
}
