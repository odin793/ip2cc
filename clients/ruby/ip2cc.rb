class Ip2cc
  
  VALID_IP = Regexp.new('^'+(['(?:(?:2[0-4]|1\d|[1-9])?\d|25[0-5])']*4).join('\.')+'$')
  
  def initialize(filename)
    @filename = filename
  end
  
  def [](ip)
    return nil unless ip =~ VALID_IP
    parts = ip.split('.').map {|part| part.to_i}
    open(@filename) do |f|
      offset = 0
      parts.each do |part|
       start = offset + part * 4
       f.seek(start)
       value = f.read(4)
       if value[0, 2] == "\xFF\xFF" then
         return nil if value[2, 2] == "\x00\x00"
         return value[2, 2]
       end
       offset = value.unpack('N')[0]
      end
    end
    raise 'broken db'
  end
  
end

if __FILE__ == $0 then
  db = Ip2cc.new '../../ip2cc.db'
  if ARGV.length == 1 then
    puts(db[ARGV[0]])
  else
    ARGV.each{ |ip| puts "#{ip} #{db[ip]}" }
  end
end