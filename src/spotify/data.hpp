#pragma once
#include <string_view>

namespace spotify {

namespace data {

constexpr std::string_view callback_payload = R"(
<!DOCTYPE html>
<html>
	<head>
		<style>
			* {
				padding: 0;
				margin: 0;
			}

			body {
				background: #222;
				color: #eee;
			}

			main {
				margin: 0 auto;
				width: 1200px;
				text-align: center;
				padding: 2rem;
			}
		</style>
	</head>
	<body>
		<main>
			<h1>
				<script>
					window.close();
				</script>
				Auth complete, you may close this tab now
			</h1>
		</main>
	</body>
</html>)";

constexpr std::string_view regular_payload = R"(
<!DOCTYPE html>
<html>
	<head>
		<style>
			* {
				padding: 0;
				margin: 0;
			}

			body {
				background: #222;
				color: #eee;
			}

			main {
				margin: 0 auto;
				width: 1200px;
				text-align: center;
				padding: 2rem;
			}
		</style>
	</head>
	<body>
		<main>
		</main>
	</body>
</html>)";

constexpr std::string_view scopes = "playlist-read-collaborative%20playlist-read-private%20playlist-modify-private%20playlist-modify-public%20user-follow-read%20user-follow-modify%20user-library-modify%20user-library-read%20user-modify-playback-state%20user-read-currently-playing%20user-read-playback-state%20user-read-playback-position%20user-read-private%20user-read-recently-played";
 
}

}
