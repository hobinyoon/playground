package main

import (
	"context"
	"fmt"
	"log"
	"os"

	"cloud.google.com/go/bigquery"
	"google.golang.org/api/iterator"
	"google.golang.org/api/option"
)

var gcpProjectId = os.Getenv("GCP_PROJECT_ID")
var gcpCredentialPath = os.Getenv("GCP_CREDENTIAL_PATH")

func main() {
	ctx := context.Background()
	client, err := bigquery.NewClient(ctx, gcpProjectId, option.WithCredentialsFile(gcpCredentialPath))
	if err != nil {
		log.Fatalf("failed to create bigquery client: %v", err)
	}

	queryStr := fmt.Sprintf(`
	    SELECT CAST(TRUNC(EXTRACT(YEAR FROM dischtime), -1) AS INT) AS t, p.gender AS g, COUNT(*)
        FROM %s AS a JOIN %s AS p ON a.subject_id = p.subject_id
        GROUP BY t, g ORDER BY t, g`,
		"`physionet-data.mimiciii_demo.admissions`", "`physionet-data.mimiciii_demo.patients`")
	q := client.Query(queryStr)
	it, err := q.Read(ctx)
	if err != nil {
		log.Fatalf("failed to read the query result: %v", err)
	}

	// Memory consumption: We store the entire result in memory and process,
	// which should be okay because the number of records should be bound to
	// the number of items displayed in one screen.

	// m is a map of {time, gender} -> admissions
	m := make(map[int64]map[string]int64)
	var first bool = true
	var time_min, time_max int64
	for {
		var cols []bigquery.Value
		err := it.Next(&cols)
		if err == iterator.Done {
			break
		}
		if err != nil {
			log.Fatalf("failed to advance iterator: %v", err)
		}

		time := cols[0].(int64)
		gender := cols[1].(string)
		population := cols[2].(int64)
		// Note: Add recover() in case any of these panics: https://go.dev/blog/defer-panic-and-recover

		if m[time] == nil {
			m[time] = make(map[string]int64)
		}
		m[time][gender] = population

		if first {
			time_min = time
			first = false
		}
		time_max = time
	}

	// Report missing or under-populated subgroups
	var sum_admissions int64 = 0
	var non_empty_groups int64 = 0
	for _, v := range m {
		for _, admissions := range v {
			sum_admissions += admissions
			non_empty_groups++
		}
	}
	// Average admissions
	avg_admissions := float64(sum_admissions) / float64(non_empty_groups)
	fmt.Printf("sum_admissions=%v avg_admissions=%v\n", sum_admissions, avg_admissions)

	var population_bias_threshold float64 = 0.8

	// Note: Parameterize
	// Note: Use time duration
	var time_window_size int64 = 10
	// This could be pre-configured - getting the item list from the table (a) might take too long
	// and (b) might miss some values
	genders := []string{"M", "F"}

	for t := time_min; t <= time_max; t += time_window_size {
		for _, g := range genders {
			population, exists := m[t][g]
			var missing_or_under_populated bool = true
			if exists {
				missing_or_under_populated = float64(population) < avg_admissions*population_bias_threshold
			}
			fmt.Println(t, g, population, missing_or_under_populated)
		}
	}
}
