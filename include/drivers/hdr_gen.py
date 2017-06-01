import sys
import os

# Example: python hdr_gen.py drv_ccs811 ccs811_regs.def


def next_field_index_get(curr_idx, inp_fieldsv):
	tmp_idx = curr_idx + 1
	
	while tmp_idx < len(inp_fieldsv):
		params = inp_fieldsv[tmp_idx].strip().split('|')
		if params[0] == 'f':
			break
		tmp_idx += 1
			
	return  tmp_idx


# Expands field series to actual fields.
#
# Note: Also expands field series if present.
#
# E.g from:
# r|RegisterSeries[0..1]|Generic comment for each register in the series
#	 f|RegisterField|1|1|RegisterField comment
#		 v|FieldValueOne|0|FieldValueOne comment
#		 v|FieldValueTwo|1|FieldValueTwo comment
# to:
#  [
#    ['r', 'RegisterSeries0', 'Generic comment for each register in the series'],
#    ['f', 'RegisterField', '0', '1', 'RegisterField comment'],
#    ['v', 'FieldValueOne', '0', 'FieldValueOne comment'],
#    ['v', 'FieldValueTwo', '1', 'FieldValueTwo comment'],
#    ['r', 'RegisterSeries1', 'Generic comment for each register in the series'],
#    ['f', 'RegisterField', '0', '1', 'RegisterField comment'],
#    ['v', 'FieldValueOne', '0', 'FieldValueOne comment'],
#    ['v', 'FieldValueTwo', '1', 'FieldValueTwo comment'],
#  ]
def field_series_expand(field_idx, inp_fieldsv):
	result = []
	params = inp_fieldsv[field_idx].strip().split('|')
	range_start = int(params[1][params[1].find('[') + 1:params[1].find('.')], 10)
	range_end   = int(params[1][params[1].find('.') + 2:params[1].find(']')], 10)

	if range_start < range_end:
		current_range = range(range_start, range_end + 1)
	else:
		current_range = reversed(range(range_end, range_start + 1))

	for i in current_range:
		curr_idx = field_idx
		params = inp_fieldsv[curr_idx].strip().split('|')
		params[1] = params[1][:params[1].find('[')] + \
				str(i) + \
				params[1][params[1].find(']') + 1:]
		params[2] = str(i * int(params[3], 10))
		result.append(params)
		curr_idx += 1
		while curr_idx < len(inp_fieldsv):
			params = inp_fieldsv[curr_idx].strip().split('|')
			if params[0] == 'f':
				break
			result.append(params)
			curr_idx += 1

	return result


# Expands register series to actual fields.
#
# E.g from:
#	 f|RegisterFieldSeries[0..1]|1|1|Generic comment for each field in the series
#		 v|FieldValueOne|0|FieldValueOne comment
#		 v|FieldValueTwo|1|FieldValueTwo comment
# to:
#  [
#    ['f', 'RegisterFieldSeries0', '0', '1', 'Generic comment for each field in the series'],
#    ['v', 'FieldValueOne', '0', 'FieldValueOne comment'],
#    ['v', 'FieldValueTwo', '1', 'FieldValueTwo comment'],
#    ['f', 'RegisterFieldSeries1', '1', '1', 'Generic comment for each field in the series'],
#    ['v', 'FieldValueOne', '0', 'FieldValueOne comment'],
#    ['v', 'FieldValueTwo', '1', 'FieldValueTwo comment'],
#  ]
def reg_series_expand(field_idx, inp_fieldsv):
	result = []
	params = inp_fieldsv[field_idx].strip().split('|')
	range_start = int(params[1][params[1].find('[') + 1:params[1].find('.')], 10)
	range_end   = int(params[1][params[1].find('.') + 2:params[1].find(']')], 10)
	#print repr(params)
	#print 'regss', field_idx, range_start, range_end
	if range_start < range_end:
		current_range = range(range_start, range_end + 1)
	else:
		current_range = reversed(range(range_end, range_start + 1))
	for i in current_range:
		sub_result = []
		curr_idx = field_idx
		params = inp_fieldsv[curr_idx].strip().split('|')
		#print curr_field
		params[1] = params[1][:params[1].find('[')] + \
				str(i) + \
				params[1][params[1].find(']') + 1:]
		#print params
		sub_result.append(params)
		curr_idx += 1
		while curr_idx < len(inp_fieldsv):
			params = inp_fieldsv[curr_idx].strip().split('|')
			if params[0] == 'f' and params[1].find('[') != -1:
				for field in field_series_expand(curr_idx, inp_fieldsv):
					sub_result.append(field)
				curr_idx = next_field_index_get(curr_idx, inp_fieldsv)
			else:
				sub_result.append(params)
				curr_idx += 1
		#print repr(sub_result)
		result.append(sub_result)
	#print result
	return result

	
def raw_data_display(regsv):
	for reg in regsv:
		for field in reg:
			if field[0] == 'r':
				print repr(field)
			elif field[0] == 'f':
				print '  ' + repr(field)
			else:
				print '    ' + repr(field)


if len(sys.argv) != 3:
	print 'Usage: python ' + sys.argv[0] + ' <module name> <register definition file>'
	sys.exit()

fh = open(sys.argv[2], "r")
inp_str = ''.join(fh.readlines())

# Convert line-endings to double-pipes (works for both DOS and UNIX line-endings)
inp_str = inp_str.replace('\n', '||').replace('\r', '||').replace('||||', '||')
# Separate registers with tripple-pipes.
inp_str = inp_str.replace('||r|', '|||r|')


# Build register raw data.
inp_reg_idx = 0
reg_idx = -1
inp_regv = inp_str.strip().split('|||')
regsv = []
while  inp_reg_idx < len(inp_regv):
	fieldsv = []
	field_idx = 0
	inp_fieldsv = inp_regv[inp_reg_idx].strip().split('||')
	while field_idx < len(inp_fieldsv):
		params = inp_fieldsv[field_idx].strip().split('|')
		if params[0] == 'r':
			if params[1].find('[') != -1:
				# Handles register series with or without field series.
				#
				# Without field series:
				# r|RegisterSeries[0..2]|Generic comment for each register in the series
				#	 f|RegisterField|1|1|RegisterField comment
				#		 v|FieldValueOne|0|FieldValueOne comment
				#		 v|FieldValueTwo|1|FieldValueTwo comment
				#		
				# With field series:
				# r|RegisterSeries[0..2]|Generic comment for each register in the series
				#	 f|RegisterFieldSeries[0..2]|1|1|Generic comment for each field in the series
				#		 v|FieldValueOne|0|FieldValueOne comment
				#		 v|FieldValueTwo|1|FieldValueTwo comment
				for reg in reg_series_expand(field_idx, inp_fieldsv):
					regsv.append(reg)
					reg_idx += 1
				field_idx = len(inp_fieldsv)
			else:
				regsv.append([])
				reg_idx += 1
				regsv[reg_idx].append(params)
				field_idx += 1
		elif params[0] == 'f':
			if params[1].find('[') != -1:
				# Handles field series.
				#
				# r|RegisterSeries[0..2]|Generic comment for each register in the series
				#	 f|RegisterFieldSeries[0..2]|1|1|Generic comment for each field in the series
				#		 v|FieldValueOne|0|FieldValueOne comment
				#		 v|FieldValueTwo|1|FieldValueTwo comment
				for field in field_series_expand(field_idx, inp_fieldsv):
					regsv[reg_idx].append(field)
				field_idx = next_field_index_get(field_idx, inp_fieldsv)
			else:
				regsv[reg_idx].append(params)
				field_idx += 1
		elif params[0] == 'v':
			regsv[reg_idx].append(params)
			field_idx += 1
		else:
			print 'ERROR: Infalid field type!'
			sys.exit()
	inp_reg_idx += 1


# Un-comment the code below to output the rawdata.	
#~ raw_data_display(regsv)
#~ sys.exit()


db = {}
db['module'] = os.path.basename(sys.argv[1])
db['registers'] = []

# Create register database.
for reg in regsv:
	for field in reg:
		if field[0] == 'r':
			last_added_reg = {'name':field[1], 'descr':field[2], 'fields': []}
			db['registers'].append(last_added_reg)
		if field[0] == 'f':
			length = len('(1 << ' + db['module'] + '_' + last_added_reg['name'] + '_' + field[1] + '_Pos)')
			last_added_field = {'name':field[1], 'value_name_max_length': 3, 'value_max_length': length, 'pos': field[2], 'msk': field[3], 'descr':field[4], 'values': []}
			last_added_reg['fields'].append(last_added_field)
		if field[0] == 'v':
			last_added_value = {'name':field[1], 'val': field[2], 'descr':field[3]}
			if len(last_added_value['name']) > last_added_field['value_name_max_length']:
				last_added_field['value_name_max_length'] = len(last_added_value['name'])
			if len(last_added_value['val']) > last_added_field['value_max_length']:
				last_added_field['value_max_length'] = len(last_added_value['val'])
			last_added_field['values'].append(last_added_value)


# Print header file.
header_file_define = db['module'].upper() + '_BITFIELDS' + '_H__'
print '#ifndef ' + header_file_define
print '#define ' + header_file_define
			
for reg in db['registers']:
	print ''
	print ''
	print '/* Register: ' + reg['name'].upper() + '. */'
	print '/* Description: ' + reg['descr'] + '. */'
	for field in reg['fields']:
		print ''
		print ''
		print '/* Field ' + field['name'].upper() + ': ' + field['descr'] + '. */'

		base_str  = (db['module'] + '_' + reg['name'] + '_' + field['name']).upper()		
		pos_str   = base_str + '_Pos'.ljust(field['value_name_max_length'] + 2)
		msk_str   = base_str + '_Msk'.ljust(field['value_name_max_length'] + 2)
		
		value_str = ('(' + field['pos'] + ')').ljust(field['value_max_length'] + 3)
		print '#define ' + pos_str + value_str  + '/*!< The position of the field. */'
		
		value_str = ('(' + hex((1 << int(field['msk'], 10)) - 1) + ' << ' + pos_str.rstrip() + ')').ljust(field['value_max_length'] + 3)
		print '#define ' + msk_str + value_str + '/*!< The mask of the field. */'
		
		for value in field['values']:
			print '#define ' + (db['module'] + '_' + reg['name'] + '_' + field['name']).upper() + '_' + value['name'].ljust(field['value_name_max_length'] + 1) + \
				('(' + value['val'] + ')').ljust(field['value_max_length'] + 3) + '/*!< ' + value['descr'] + '. */'

print ''
print '#endif // ' + header_file_define
