require 'rake/clean'
require 'vendor/cmock/lib/cmock'

CLOBBER.include('tests/**/*Runner.c', 'tests/mocks/*')

cmock_defaults = {:mock_path => 'tests/mocks', :plugins => [:ignore]}

desc "Generate mocks"
task :mocks do
#  CMock.new(cmock_defaults).setup_mocks(['include/NFC.h',
#                                         'include/ONFI.h',
#                                         'include/NAND.h'])
end

desc "Generate runners"
task :runners do
  for src in Dir["tests/**/test_*.c"] do
    if (!(src =~ /^.+_Runner\.c$/)) && TRUE
      sh "ruby vendor/cmock/vendor/unity/auto/generate_test_runner.rb #{src}"
    end
  end
end

desc "Generate all objects"
task :default => [:mocks, :runners]

desc "commit changes in git repository"
task :git, :m do |t, args|
  args.with_defaults(:m => "source code updated")
  sh "git add -A && git commit -m'#{args.m}'"
end
