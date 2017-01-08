import cmd, requests, json

HEADERS = {"X-API-Key":'d7b822df7f5743e1a001f5aca58587bd'}
# This key is needed in the HTTP header of our requests
# The key is from my Bungie.net account after I regestered to use the API

base_url = "https://www.bungie.net/platform/Destiny/"
xur_url = "https://www.bungie.net/Platform/Destiny/Advisors/Xur/?definitions=true"
activity_url = "https://www.bungie.net/Platform/Destiny/Advisors/V2/?definitions=true"

#f = open('advisor.txt', 'w') # .txt files used at advisor pages
#f2 = open('manifest.txt', 'w')	# .txt file that looks at the manifest pages

# In do_nightfall, file output commands are commented out. Uncomment them to see
# an example of the JSON advisor and manifest dictionaries.

class GhostShell(cmd.Cmd):

	def do_xur(self, args):
		"""\tPrints Xur's inventory. Only works on Friday and Saturday!"""
		
		hashType = "6" # DestinyDefinitionType to navigate manifest file

		print "\nGetting Xur's inventory..."

		# Make a HTTP request to specified URL with our key in the header
		r = requests.get(xur_url, headers=HEADERS)

		# Print the connection status
		connect = r.json()['ErrorStatus']
		print "Connection status: " + connect + "\n"

		# Go through JSON dictionaries to grab each item
		for cat in r.json()['Response']['data']['saleItemCategories']:
			saleItems = cat['saleItems']
			for myItem in saleItems:
				hashID = str(myItem['item']['itemHash'])
				hashReqString = base_url + "Manifest/" + hashType + "/" + hashID

				r2 = requests.get(hashReqString, headers=HEADERS)

				item_name = r2.json()['Response']['data']['inventoryItem']['itemName']
				print item_name
				item_type = r2.json()['Response']['data']['inventoryItem']['itemTypeName']
				item_tier = r2.json()['Response']['data']['inventoryItem']['tierTypeName']
				print item_tier + " " + item_type + "\n"
		
		#Xur's next refresh
		refresh_date = r.json()['Response']['data']['nextRefreshDate']
		print "Xur will return on " + refresh_date + "\n"
		
	
	def do_nightfall(self, args):
		"""Prints the weekly Nightfall activity and its modifiers."""

		hashType = "1"

		print "\nGetting Nightfall info..."
		r = requests.get(activity_url, headers=HEADERS)

		#f.write(json.dumps(r.json(), indent=4)) # you need to know what's in the dictionary to parse it

		connect = r.json()['ErrorStatus']
		print "Connection status: " + connect + "\n"

		nightfallHash = r.json()['Response']['data']['activities']['nightfall']['display']['activityHash']
		hashID = str(nightfallHash)
		hashReqString = base_url + "Manifest/" + hashType + "/" + hashID

		r2 = requests.get(hashReqString, headers=HEADERS)
		#f2.write(json.dumps(r2.json(), indent=4)) # a copy of the manifest file for the same reason

		activity_name = r2.json()['Response']['data']['activity']['activityName']
		print "This week's Nightfall is " + activity_name + "\n"

		print "The modifiers are "
		
		for cat in r.json()['Response']['data']['activities']['nightfall']['extended']['skullCategories']:
			myMod = cat['skulls']
			for modifiers in myMod:
				output = modifiers['displayName']
				print output

		refresh_date = r.json()['Response']['data']['activities']['nightfall']['status']['expirationDate']
		print "\nThe Nightfall will reset at " + refresh_date + "\n"


	def do_weeklycrucible(self, args):
		"""Prints the weekly Crucible playlist."""

		hashType = "1"

		print "\nGetting weekly Crucible info..."
		r = requests.get(activity_url, headers=HEADERS)

		connect = r.json()['ErrorStatus']
		print "Connection status: " + connect + "\n"

		crucibleHash = r.json()['Response']['data']['activities']['weeklycrucible']['display']['activityHash']
		hashID = str(crucibleHash)
		hashReqString = base_url + "Manifest/" + hashType + "/" + hashID

		r2 = requests.get(hashReqString, headers=HEADERS)

		activity_name = r2.json()['Response']['data']['activity']['activityName']
		print "The weekly Crucible playlist is " + activity_name + "\n"

		refresh_date = r.json()['Response']['data']['activities']['weeklycrucible']['status']['expirationDate']
		print "The weekly playlist will reset at " + refresh_date + "\n"


	def do_dailycrucible(self, args):
		"""Prints the daily Crucible playlist."""

		hashType = "1"

		print "\nGetting daily Crucible info..."
		r = requests.get(activity_url, headers=HEADERS)

		connect = r.json()['ErrorStatus']
		print "Connection status: " + connect + "\n"

		crucibleHash = r.json()['Response']['data']['activities']['dailycrucible']['display']['activityHash']
		hashID = str(crucibleHash)
		hashReqString = base_url + "Manifest/" + hashType + "/" + hashID

		r2 = requests.get(hashReqString, headers=HEADERS)

		activity_name = r2.json()['Response']['data']['activity']['activityName']
		print "The daily Crucible playlist is " + activity_name + "\n"

		refresh_date = r.json()['Response']['data']['activities']['dailycrucible']['status']['expirationDate']
		print "The daily playlist will reset at " + refresh_date + "\n"


	def do_dailymission(self, args):
		"""Prints the daily Story mission."""

		hashType = "1"

		print "\nGetting daily Story info..."
		r = requests.get(activity_url, headers=HEADERS)

		connect = r.json()['ErrorStatus']
		print "Connection status: " + connect + "\n"

		chapterHash = r.json()['Response']['data']['activities']['dailychapter']['display']['activityHash']
		hashID = str(chapterHash)
		hashReqString = base_url + "Manifest/" + hashType + "/" + hashID

		r2 = requests.get(hashReqString, headers=HEADERS)

		activity_name = r2.json()['Response']['data']['activity']['activityName']
		print "The daily Story mission is " + activity_name + "\n"

		refresh_date = r.json()['Response']['data']['activities']['dailychapter']['status']['expirationDate']
		print "The daily mission will reset at " + refresh_date + "\n"


	def do_quit(self, args):
		"""\tQuits ghostshell."""
		print "Quitting ghostshell."
		raise SystemExit


if __name__ == '__main__':
	prompt = GhostShell()
	prompt.prompt = '$[ghostshell] '
	prompt.cmdloop('Type help for commands. Starting ghostshell...')
