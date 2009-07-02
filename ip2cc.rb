VALID_IP = Regexp.new('^'+(['(?:(?:2[0-4]|1\d|[1-9])?\d|25[0-5])']*4).join('\.')+'$')

class IP2CC
  
  def initialize(filename)
    @filename = filename
  end
  
  def get(ip)
    throw :invalid_ip unless ip =~ VALID_IP
    open(@filename) do |f|
      offset = 0
      ip.split('.').each do |part|
       start = offset + part.to_i * 4
       f.seek start
       value = f.read 4
       if value.slice(0, 2) == "\xFF\xFF" then
         throw :not_found if value.slice(2, 2) == "\x00\x00"
         return value.slice(2, 2)
       end
       offset = value.unpack('N')[0]
      end
    end
    throw :broken_db
  end
  
end

if __FILE__ == $0 then
  db = IP2CC.new 'ip2cc.db'
  if ARGV.length == 1 then
    puts db.get(ARGV[0])
  else
    ARGV.each{ |ip| puts "#{ip} #{db.get(ip)}" }
  end
end