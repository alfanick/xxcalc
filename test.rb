#!/usr/bin/ruby

n = [ARGV[0].to_i, 10].max
k = [ARGV[1].to_i, 1].max

k.times do |i|
  puts (1..n).to_a.join('+') + "-" + (1..n).to_a.join('-')
end
