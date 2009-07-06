class IP2CC {
    def file
    IP2CC(filename){
         file = new RandomAccessFile(filename, "r")
    }

    synchronized String getCountry(ip) {
        def offset = 0
        def buf = new byte[4]
        def int_buf = new int[4]
        def readed = 0
        try {
            for (part in ip.split("\\.")) {
                int start = offset + Integer.parseInt(part) * 4
                file.seek(start)
                if ((readed = file.read(buf)) != -1) {
                    buf.eachWithIndex{item, i ->
                        if (item < 0 )
                            int_buf[i] = 256 - Math.abs((int)item)
                        else 
                            int_buf[i] = item
                    }
                    if (int_buf[0] == 255 && int_buf[1] == 255) {
                        if (int_buf[2] == 0 & int_buf[3] == 0) {
                            file.close()
                            throw new Exception(ip)
                        } 
                        file.close()
                        return new String(buf, 2,2)
                    }
                } else {file.close(); return "end of file"}
                offset = int_buf[0] << 24 | int_buf[1] << 16 | int_buf[2] << 8 | int_buf[3]
            }
        } catch (IOException) {file.close(); return "got problems with file"}
    }

    public static void main(String[] args) {
        def ip2cc = new IP2CC("./ip2cc.db")
        def value = ip2cc.getCountry(args[0])
        println(value)
    }
}
