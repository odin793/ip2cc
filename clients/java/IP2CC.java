import java.io.RandomAccessFile;
import java.io.IOException;
import java.io.FileNotFoundException;

class IP2CC {

    private RandomAccessFile file;

    IP2CC(String filename) throws FileNotFoundException {
        this.file = new RandomAccessFile(filename, "r");
    }

    public synchronized String get_country(String ip) throws IOException {
        int offset = 0;
        byte[] buf = new byte[4];
        int[] int_buf = new int[4];
        int readed = 0;
        String result = "nothing found for ip=" + ip;
        for(String part : ip.split("\\.")){
            int start = offset + Integer.parseInt(part)*4;
            file.seek(start);
            if((readed = file.read(buf)) != -1){
                for ( int i=0; i < buf.length; i++){
                    if (buf[i] < 0)
                        int_buf[i] = 256 + (int)buf[i];
                    else
                        int_buf[i] = (int)buf[i];
                }
                offset = int_buf[0] << 24 | int_buf[1] << 16 | int_buf[2] << 8 | int_buf[3];
                if (int_buf[0] == 255 && int_buf[1] == 255){
                    if (int_buf[2] == 0 && int_buf[3] == 0) 
                        break;
                    result = new String(int_buf, 2 ,2);
                    break;
                }
            } else { break;}
        }
        file.close();
        return result;
    }

    public static void main(String[] args) {
        try {
            IP2CC ip2cc = new IP2CC("./ip2cc.db");
            System.out.println(ip2cc.get_country(args[0]));
        } catch (FileNotFoundException e) { 
            System.out.println("db file not found"); 
        } catch (IOException e) {
            System.out.println("problems with I/O"); 
        }
    }
}
