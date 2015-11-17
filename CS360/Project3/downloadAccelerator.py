import argparse
import os
import requests
import threading
import time

class Downloader:
	def __init__(self):
		self.args = None
		self.url = 'http://www.jordantaylor.info'
		self.filename = 'index.html'
		self.thread_count = 3
		self.parse_arguments()

	def parse_arguments(self):
		parser = argparse.ArgumentParser(prog='Download Accelerator', description='A simple script that downloads a file using multiple threads', add_help=True)
		parser.add_argument('-n', '--number_of_threads', type=int, help='Specify the number of threads to use',default=3)
		parser.add_argument('url', help='The url of the file to be donwloaded',default='http://www.jordantaylor.info')
		args = parser.parse_args()
		self.url = args.url        
		split = self.url.split('/')
		# print split[-1]
		if split[-1] != '':
			self.filename = split[-1]
		self.thread_count = int(args.number_of_threads)

	def download(self): 
		r = requests.head(self.url)
		try:
			length = int(r.headers['content-length'])
			increment = length / self.thread_count
			shared = SharedFile(self.filename, length)
			if length > 0:
				threads = []
				start_time = time.time()
				starting_bit = 0
				for i in range(0,self.thread_count):
					bit_range = 'bytes=' + str(starting_bit) + '-' + str(starting_bit + increment)
					t = DownThread(self.url, self.filename, starting_bit, bit_range, shared)
					starting_bit = starting_bit + increment + 1
					threads.append(t)
				for t in threads:
					t.start()
				for t in threads:
					t.join()

				total_time = time.time() - start_time
				print "%s %d %d %f" % (self.url, self.thread_count, length, total_time)
		except KeyError, e:
			print "Error: Page redirected and key (" + str(e) + ") was not found in headers"


		

class DownThread(threading.Thread):
	def __init__(self,url,filename,starting_bit,bit_range,shared_file):
		self.url = url
		self.starting_bit = starting_bit
		self.bit_range = bit_range
		self.filename = filename
		self.shared_file = shared_file
		threading.Thread.__init__(self)

	def run(self):
		# print '%s Range is %s' % (self.getName(), self.bit_range)
		headers = {'Range': self.bit_range, 'Accept-Encoding': 'identity'}
		start_time = time.time()
		r = requests.get(self.url, headers=headers)
		# rate = int(r.headers['content-length']) / (time.time() - start_time)/1024/1024
		# if rate < 1:
			# rate *= 1024
			# print "%s's download rate: %f KB/s" % (self.getName(), rate)
		# else:
			# print "%s's download rate: %f MB/s" % (self.getName(), rate)
		# print 
		# print "%s is trying to write to file" % self.getName()
		self.shared_file.shared_write(r.content, self.starting_bit)
		# print '-------- %s has finished writing to file --------' % self.getName()

class SharedFile:
	def __init__(self,filename,filesize):
		myfile = open(filename, 'wb')
		myfile.seek(filesize-1)
		myfile.write('\0')
		self.myfile = myfile
		self.sem = threading.Semaphore()

	def shared_write(self,content,starting_bit):        
		self.sem.acquire()
		self.myfile.seek(starting_bit)
		self.myfile.write(content)
		self.sem.release()


if __name__ == '__main__':
	d = Downloader()
	d.download()
